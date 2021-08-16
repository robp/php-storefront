<?php
  function next_seq ($table) {
    global $sock;

    $query = "INSERT INTO $table VALUES (NULL)";

    if (!$sql_result = mysql_query($query, $sock)) {
      echo mysql_error();
      html_exit();
    }

    return mysql_insert_id($sock);
  }

  function generate_basket_id () {
    global $sock;

    srand((double)microtime()*1000000);
    $digits = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    $in_use = 1;

    while ($in_use) {
      $basket_id = "";

      for ($i = 0; $i < BASKET_ID_LENGTH; $i++) {
        $basket_id .= $digits[rand(0, strlen($digits))];
      }

      $query = "SELECT id FROM orders 
                WHERE basket_id = '$basket_id'";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      if (mysql_num_rows($sql_result) == 0)
        $in_use = 0;
    }

    return $basket_id;
  }

  function validateCC ($ccnum,  $type  =  'unknown') {
    $type = strtolower($type);
    $ccnum = ereg_replace('[-[:space:]]', '', $ccnum);

    //Do  type  specific  check
    if ($type == 'unknown') {
      // Skip type specific checks
    }
    elseif ($type == 'mastercard') {
      if (strlen($ccnum) != 16 || !ereg('^5[1-5]', $ccnum))
        return 0;
    }
    elseif ($type == 'visa') {
      if ((strlen($ccnum) != 13 && strlen($ccnum) != 16) ||
           substr($ccnum, 0, 1) != '4')
        return 0;
    }
    elseif ($type == 'amex') {
      if (strlen($ccnum) != 15 || !ereg('^3[47]', $ccnum))
        return 0;
    }
    elseif ($type == 'discover') {
      if (strlen($ccnum) != 16 || substr($ccnum, 0, 4) != '6011')
        return 0;
    }
    else {
      // invalid type entered
      return -1;
    }


    //  Start  MOD  10  checks

    $dig = toCharArray($ccnum);
    $numdig = sizeof ($dig);
    $j = 0;
    for ($i = ($numdig - 2); $i >= 0; $i -= 2) {
      $dbl[$j] = $dig[$i] * 2;
      $j++;
    }
    $dblsz = sizeof($dbl);
    $validate = 0;
    for ($i = 0; $i < $dblsz; $i++) {
      $add = toCharArray($dbl[$i]);
      for ($j = 0; $j < sizeof($add); $j++) {
        $validate += $add[$j];
      }
      $add = '';
    }
    for ($i = ($numdig - 1); $i >= 0; $i -= 2) {
      $validate += $dig[$i];
    }
    if (substr($validate, -1, 1) == '0')
      return 1;
    else
      return  0;
  }

  // takes a string and returns an array of characters
  function toCharArray($input) {
    $len = strlen($input);
    for ($j = 0; $j < $len; $j++) {
      $char[$j] = substr($input, $j, 1);
    }
    return ($char);
  }

  function safe_CC($cc_number) {
    $tmpcc = strrev($cc_number);
    $foo = strrev($cc_number);
    for ($x = 4; $x < strlen($tmpcc); $x++) {
      $tmpcc[$x] = "x";
    }
    $cc_plain = strrev($tmpcc);

    return ($cc_plain);
  }

  // List the parent categories in a <option> type format
  function list_categories_select ($selected, $show_id) {
    global $sock;

    $query = "SELECT * FROM categories
              ORDER BY name";

    if (!$sql_result = mysql_query($query, $sock)) {
      echo mysql_error();
      html_exit();
    }

    for ($x = 0; $x < mysql_num_rows($sql_result); $x++) {
      $category = new Category(mysql_fetch_row($sql_result), 0);

      echo "<option value=\"$category->id\"" . ($category->id == $selected ? " selected" : "") . ">";

      if ($show_id)
        echo "$category->id. ";

      echo "$category->name";
    }
  }

  function list_categories_recursive ($id) {
    global $sock;

    $query = "SELECT * FROM categories
              WHERE parent_id = $id
              ORDER BY name";

    if (!$sql_result = mysql_query($query, $sock)) {
      echo mysql_error();
      html_exit();
    }

    for ($i = 0; $i < mysql_num_rows($sql_result); $i++) {
      $category = new Category(mysql_fetch_row($sql_result), 0);

      if ($category->template_id)
        $template = new Template (0, $category->template_id);

      printf("<dl><dt><img src=\"img/dir.gif\" alt=\"+\"> %d. <b><a href=\"modcat.phtml?id=%d\">%s</a></b> <i>(<a href=\"index.phtml?s=inventory&f=viewitems.phtml?c=%d\" target=\"_top\">%d items</a>)</i>%s<br clear=left>\n",
        $category->id, $category->id, $category->name, 
        $category->id, $category->num_items,
        ($category->template_id ? " [<a href=\"index.phtml?s=templates&f=viewtemplate.phtml?id=" . $template->id . "\" target=\"_top\">" . $template->filename . "</a>]" : ""));
      list_categories_recursive($category->id);
      echo "</dl>\n";
    }
  }

  function format_money ($val) {
    $money = sprintf("%.2f", $val);

    for ($i = strlen($money) - 1; $i >= 0; $i--)
      $temp .= $money[$i];

    $money = $temp[0];
    $money .= $temp[1];
    $money .= $temp[2];

    for ($i = 3; $i < strlen($temp); $i++) {
      if ( (($i - 3) % 3) == 0 && $i != 3)
        $money .= ",";

      $money .= $temp[$i];
    }

    for ($i = strlen($money) - 1; $i >= 0; $i--)
      $temp2 .= $money[$i];

    return $temp2;
  }

  // Check to see if a value is in the specified
  // database table. Return 0 if not, 1 if so.
  function db_item_exists ($label, $value, $table) {
    global $sock;

    $query = "SELECT $label FROM $table
              WHERE $label = $value";

    if (!$sql_result = mysql_query($query, $sock)) {
      echo mysql_error();
      html_exit();
    }
     
    if (!mysql_num_rows($sql_result))
      return 0;
    else   
      return mysql_num_rows($sql_result);
  }        

  // Connect to the SQL server and select the database
  function db_connect () {
    if (!$sock = mysql_connect(DATABASE_HOST,DATABASE_USER,DATABASE_PASSWD)) {
      echo mysql_error();
      exit();          
    }
    if (!mysql_select_db(DATABASE_NAME, $sock)) {
      echo mysql_error();
      exit();          
    }

    return $sock;
  }

  function get_weight_by_id ($id) {
    if ($id == "0")
      return "kg";
    else
      return "lb";
  }

  function center($text, $linelen) {
    $padlen = ($linelen - strlen($text)) / 2;
    for ($i = 0; $i < $padlen; $i++) $pad .= " ";
    return $pad . $text;
  }

  function list_templates_select ($selected, $show_id) {
    global $sock;

    $query = "SELECT * FROM templates
              ORDER BY filename";

    if (!$sql_result = mysql_query($query, $sock)) {
      echo mysql_error();
      html_exit();
    }

    for ($x = 0; $x < mysql_num_rows($sql_result); $x++) {
      $template = new Template(mysql_fetch_row($sql_result), 0);
      echo "<option value=\"$template->id\"" . ($template->id == $selected ? " selected" : "") . ">";
      if ($show_id)
        echo "$template->id. ";
      echo "$template->filename";
    }
  }

  function list_product_templates_select ($selected, $show_id) {
    global $sock;

    $query = "SELECT * FROM templates
              WHERE type = 0
              ORDER BY filename";

    if (!$sql_result = mysql_query($query, $sock)) {
      echo mysql_error();
      html_exit();
    }

    for ($x = 0; $x < mysql_num_rows($sql_result); $x++) {
      $template = new Template(mysql_fetch_row($sql_result), 0);
      echo "<option value=\"$template->id\"" . ($template->id == $selected ? " selected" : "") . ">";
      if ($show_id)
        echo "$template->id. ";
      echo "$template->filename";
    }
  }

  function list_category_templates_select ($selected, $show_id) {
    global $sock;

    $query = "SELECT * FROM templates
              WHERE type = 1
              ORDER BY filename";

    if (!$sql_result = mysql_query($query, $sock)) {
      echo mysql_error();
      html_exit();
    }

    for ($x = 0; $x < mysql_num_rows($sql_result); $x++) {
      $template = new Template(mysql_fetch_row($sql_result), 0);
      echo "<option value=\"$template->id\"" . ($template->id == $selected ? " selected" : "") . ">";
      if ($show_id)
        echo "$template->id. ";
      echo "$template->filename";
    }
  }

  function list_options_select ($id, $option_id) {
    global $sock;

    $option_name = "option" . $option_id;

    $query = "SELECT DISTINCT " . $option_name . " FROM items
              WHERE id = " . $id . "
              ORDER BY " . $option_name;

    if (!$sql_result = mysql_query($query, $sock)) {
      echo mysql_error();
      html_exit();
    }

    for ($x = 0; $x < mysql_num_rows($sql_result); $x++) {
      $sql_result_row = mysql_fetch_row($sql_result);
      $option = stripslashes($sql_result_row[0]);
      $result .= "<option value=\"" . htmlspecialchars($option) . "\">$option";
    }

    return $result;
  }

  function list_options_price_select ($id) {
    global $sock;

    $query = "SELECT sku FROM items
              WHERE id = " . $id . "
              ORDER BY sku";

    if (!$sql_result = mysql_query($query, $sock)) {
      echo mysql_error();
      html_exit();
    }

    for ($x = 0; $x < mysql_num_rows($sql_result); $x++) {
      $option_str = "";
      $sql_result_row = mysql_fetch_row($sql_result);
      $item = new Item(0, $sql_result_row[0]);
      
      $result .= "<option value=\"" . $item->sku . "\">";

      for ($y = 1; $y <= 10; $y++) {
        $option_name = "option" . $y;
        if ($item->$option_name) {
          if ($option_str) $option_str .= ", ";
          $option_str .= $item->$option_name;
        }
      }

      $result .= $option_str;

      if ($option_str) $result .= ": ";
      $result .= "$" . ($item->sale_price != $item->price ? format_money($item->sale_price) : format_money($item->price));
    }

    return $result;
  }

  // Get a product's attribute by its sku
  function get_pattr_by_sku ($sku, $attr) {
    global $sock;

    $item = new Item(0, $sku);

    if ($item->$attr) {
      return $item->$attr;
    }

    return "";
  }        

  // Get a product's dynamic attribute by its sku
  function get_pdynattr_by_sku ($sku, $attr) {
    global $sock;

    $item = new Item(0, $sku);

    if ($item->id) {
      $itemdyn = new ItemDyn(0, $item->id);

      if ($itemdyn->$attr) {
        return $itemdyn->$attr;
      }
    }

    return "";
  }        

  function parse_dynamic_page ($str, $itemid, $sku, $categoryid) {
    global $sock;
    global $config;

    // Gotta fix this ad_display crap
    //$foo = ad_display(AD_LOC_ID, 0, 1);
    //$result = str_replace("%AD_DISPLAY%", $foo);

    $category = new Category (0, $categoryid);

    $str = str_replace("%STORE_NAME%", $config->store_name, $str);
      
    $str = str_replace("%DATE_WEEKDAY_SHORT%", date("D"), $str);
    $str = str_replace("%DATE_WEEKDAY_LONG%", date("l"), $str);
    $str = str_replace("%DATE_MONTH_LONG%", date("F"), $str);
    $str = str_replace("%DATE_MONTH_SHORT%", date("M"), $str);
    $str = str_replace("%DATE_DAY%", date("j"), $str);
    $str = str_replace("%DATE_YEAR_SHORT%", date("y"), $str);
    $str = str_replace("%DATE_YEAR_LONG%", date("Y"), $str);

    $str = str_replace("%STORE_URL%", $config->store_url, $str);
    $str = str_replace("%VIEWBASKET_URL%", $config->viewbasket_url, $str);
    $str = str_replace("%CHECKOUT_URL%", $config->checkout_url, $str);

    if ($category->id) {
      $str = str_replace("%CATEGORY_ID%", $category->id, $str);
      $str = str_replace("%CATEGORY_NAME%", $category->name, $str);
      $str = str_replace("%CATEGORY_DESC%", $category->desc_long1, $str);
      $str = str_replace("%CATEGORY_DESC_SHORT%", $category->desc_short, $str);
      $str = str_replace("%CATEGORY_DESC_LONG1%", $category->desc_long1, $str);
      $str = str_replace("%CATEGORY_DESC_LONG2%", $category->desc_long2, $str);
      $str = str_replace("%CATEGORY_IMG_SMALL%", $category->img_small, $str);
      $str = str_replace("%CATEGORY_IMG_MEDIUM%", $category->img_medium, $str);
      $str = str_replace("%CATEGORY_IMG_LARGE%", $category->img_large, $str);
      $str = str_replace("%IF_CATEGORY_IMG_SMALL%", ($category->img_small ? "<img border=0 src=\"$category->img_small\">" : ""), $str);
      $str = str_replace("%IF_CATEGORY_IMG_MEDIUM%", ($category->img_medium ? "<img border=0 src=\"$category->img_medium\">" : ""), $str);
      $str = str_replace("%IF_CATEGORY_IMG_LARGE%", ($category->img_large ? "<img border=0 src=\"$category->img_large\">" : ""), $str);
    }


    // If we were supplied a category id, then this is for a category 
    // template which is displaying other categories

    // If we were supplied a sku, grab the item
    // from the db
    if ($sku) {
      $item = new Item(0, $sku);
      $itemid = $item->id;
    }

    // If we were supplied an itemid, or a sku and the item was found,
    // continue to parse. Otherwise, we'll continue to the item-inspecific
    // parts at the bottom
    if ($itemid) {
      $itemdyn = new ItemDyn(0, $itemid);

      // If we were supplied a dynamic item ID, but not a sku
      // for an actual item, grab the first item in the item class
      // to use for referencing individual item variables.
      if ($itemdyn->id && !$sku) {
        $query = "SELECT sku FROM items
                  WHERE id = " . $itemdyn->id;

        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }

        if (mysql_num_rows($sql_result)) {
          $sql_result_row = mysql_fetch_row($sql_result);
          $item = new Item(0, $sql_result_row[0]);
        }
      }

      // Grab the category for this item
      $category = new Category(0, $item->category);

      if ($item->sale_price != $item->price) {
        $price_str = "<strike>" . format_money($item->price) . "</strike>";
        $price_str .= " \$" . format_money($item->sale_price) . " Sale!";
      }
      else
        $price_str = format_money($item->price);


      // At this point, we will always have at least one item, and
      // may or may not have an itemdyn.

      $str = str_replace("%PRODUCT_ID%", ($itemdyn->id ? $itemdyn->id : ""), $str);
      $str = str_replace("%PRODUCT_CATEGORY_ID%", $category->id, $str);
      $str = str_replace("%PRODUCT_CATEGORY_NAME%", $category->name, $str);
      $str = str_replace("%PRODUCT_SKU%", $item->sku, $str);
      $str = str_replace("%PRODUCT_NAME%", htmlspecialchars($item->name), $str);
      $str = str_replace("%PRODUCT_DYN_NAME%", ($itemdyn->id ? htmlspecialchars($itemdyn->name) : ""), $str);
      $str = str_replace("%PRODUCT_PRICE%", $price_str, $str);
      $str = str_replace("%PRODUCT_PST%", $item->pst, $str);
      $str = str_replace("%PRODUCT_GST%", $item->gst, $str);
      $str = str_replace("%PRODUCT_WEIGHT%", $item->weight, $str);
      $str = str_replace("%PRODUCT_WEIGHT_UNIT%", ($item->weight_type == 0 ? "kg" : "lb"), $str);

      $str = str_replace("%PRODUCT_OPTIONS_PRICE_SELECT%", ($itemdyn->id ? list_options_price_select($itemdyn->id) : ""), $str);

      $str = str_replace("%PRODUCT_OPTION_1%", $item->option1, $str);
      $str = str_replace("%PRODUCT_OPTION_2%", $item->option2, $str);
      $str = str_replace("%PRODUCT_OPTION_3%", $item->option3, $str);
      $str = str_replace("%PRODUCT_OPTION_4%", $item->option4, $str);
      $str = str_replace("%PRODUCT_OPTION_5%", $item->option5, $str);
      $str = str_replace("%PRODUCT_OPTION_6%", $item->option6, $str);
      $str = str_replace("%PRODUCT_OPTION_7%", $item->option7, $str);
      $str = str_replace("%PRODUCT_OPTION_8%", $item->option8, $str);
      $str = str_replace("%PRODUCT_OPTION_9%", $item->option9, $str);
      $str = str_replace("%PRODUCT_OPTION_10%", $item->option10, $str);

      $str = str_replace("%PRODUCT_OPTION_1_DESC%", ($itemdyn->id ? $itemdyn->option1desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_1_SELECT%", ($itemdyn->id ? list_options_select($itemdyn->id, 1) : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_1_RADIO%", ($itemdyn->id ? $itemdyn->option1desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_2_DESC%", ($itemdyn->id ? $itemdyn->option2desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_2_SELECT%", ($itemdyn->id ? list_options_select($itemdyn->id, 2) : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_2_RADIO%", ($itemdyn->id ? $itemdyn->option2desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_3_DESC%", ($itemdyn->id ? $itemdyn->option3desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_3_SELECT%", ($itemdyn->id ? list_options_select($itemdyn->id, 3) : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_3_RADIO%", ($itemdyn->id ? $itemdyn->option3desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_4_DESC%", ($itemdyn->id ? $itemdyn->option4desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_4_SELECT%", ($itemdyn->id ? list_options_select($itemdyn->id, 4) : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_4_RADIO%", ($itemdyn->id ? $itemdyn->option4desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_5_DESC%", ($itemdyn->id ? $itemdyn->option5desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_5_SELECT%", ($itemdyn->id ? list_options_select($itemdyn->id, 5) : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_5_RADIO%", ($itemdyn->id ? $itemdyn->option5desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_6_DESC%", ($itemdyn->id ? $itemdyn->option6desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_6_SELECT%", ($itemdyn->id ? list_options_select($itemdyn->id, 6) : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_6_RADIO%", ($itemdyn->id ? $itemdyn->option7desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_7_DESC%", ($itemdyn->id ? $itemdyn->option7desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_7_SELECT%", ($itemdyn->id ? list_options_select($itemdyn->id, 7) : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_7_RADIO%", ($itemdyn->id ? $itemdyn->option7desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_8_DESC%", ($itemdyn->id ? $itemdyn->option8desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_8_SELECT%", ($itemdyn->id ? list_options_select($itemdyn->id, 8) : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_8_RADIO%", ($itemdyn->id ? $itemdyn->option8desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_9_DESC%", ($itemdyn->id ? $itemdyn->option9desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_9_SELECT%", ($itemdyn->id ? list_options_select($itemdyn->id, 9) : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_9_RADIO%", ($itemdyn->id ? $itemdyn->option9desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_10_DESC%", ($itemdyn->id ? $itemdyn->option10desc : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_10_SELECT%", ($itemdyn->id ? list_options_select($itemdyn->id, 10) : ""), $str);
      $str = str_replace("%PRODUCT_OPTION_10_RADIO%", ($itemdyn->id ? $itemdyn->option10desc : ""), $str);

      $str = str_replace("%PRODUCT_DESC_SHORT%", ($itemdyn->id ? $itemdyn->desc_short : ""), $str);
      $str = str_replace("%PRODUCT_DESC_LONG1%", ($itemdyn->id ? $itemdyn->desc_long1 : ""), $str);
      $str = str_replace("%PRODUCT_DESC_LONG2%", ($itemdyn->id ? $itemdyn->desc_long2 : ""), $str);
      $str = str_replace("%PRODUCT_IMG_SMALL%", ($itemdyn->id ? $itemdyn->img_small : ""), $str);
      $str = str_replace("%PRODUCT_IMG_MEDIUM%", ($itemdyn->id ? $itemdyn->img_medium : ""), $str);
      $str = str_replace("%PRODUCT_IMG_LARGE%", ($itemdyn->id ? $itemdyn->img_large : ""), $str);
      $str = str_replace("%IF_PRODUCT_IMG_SMALL%", ($itemdyn->img_small ? "<img border=\"0\" src=\"$itemdyn->img_small\">" : ""), $str);
      $str = str_replace("%IF_PRODUCT_IMG_MEDIUM%", ($itemdyn->img_medium ? "<img border=\"0\" src=\"$itemdyn->img_medium\">" : ""), $str);
      $str = str_replace("%IF_PRODUCT_IMG_LARGE%", ($itemdyn->img_large ? "<img border=\"0\" src=\"$itemdyn->img_large\">" : ""), $str);
    }




    while (ereg("%PRODUCT_ID\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_ID(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pattr_by_sku($regs[1], "id"), $str);
    }

    while (ereg("%PRODUCT_NAME\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_NAME(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pattr_by_sku($regs[1], "name"), $str);
    }

    while (ereg("%PRODUCT_DYN_NAME\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_DYN_NAME(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "name"), $str);
    }

    while (ereg("%PRODUCT_PRICE\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_PRICE(" . $regs[1] . ")%";
      $str = str_replace($str2, format_money(get_pattr_by_sku($regs[1], "sale_price")), $str);
    }

    while (ereg("%PRODUCT_WEIGHT\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_WEIGHT(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pattr_by_sku($regs[1], "weight"), $str);
    }

    while (ereg("%PRODUCT_OPTION_1\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_1(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pattr_by_sku($regs[1], "option1"), $str);
    }

    while (ereg("%PRODUCT_OPTION_2\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_2(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pattr_by_sku($regs[1], "option2"), $str);
    }

    while (ereg("%PRODUCT_OPTION_3\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_3(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pattr_by_sku($regs[1], "option3"), $str);
    }

    while (ereg("%PRODUCT_OPTION_4\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_4(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pattr_by_sku($regs[1], "option4"), $str);
    }

    while (ereg("%PRODUCT_OPTION_5\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_5(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pattr_by_sku($regs[1], "option5"), $str);
    }

    while (ereg("%PRODUCT_OPTION_6\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_6(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pattr_by_sku($regs[1], "option6"), $str);
    }

    while (ereg("%PRODUCT_OPTION_7\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_7(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pattr_by_sku($regs[1], "option7"), $str);
    }

    while (ereg("%PRODUCT_OPTION_8\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_8(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pattr_by_sku($regs[1], "option8"), $str);
    }

    while (ereg("%PRODUCT_OPTION_9\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_9(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pattr_by_sku($regs[1], "option9"), $str);
    }

    while (ereg("%PRODUCT_OPTION_10\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_10(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pattr_by_sku($regs[1], "option10"), $str);
    }

    while (ereg("%PRODUCT_OPTION_1_DESC\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_1_DESC(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "option1desc"), $str);
    }

    while (ereg("%PRODUCT_OPTION_2_DESC\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_2_DESC(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "option2desc"), $str);
    }

    while (ereg("%PRODUCT_OPTION_3_DESC\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_3_DESC(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "option3desc"), $str);
    }

    while (ereg("%PRODUCT_OPTION_4_DESC\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_4_DESC(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "option4desc"), $str);
    }

    while (ereg("%PRODUCT_OPTION_5_DESC\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_5_DESC(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "option5desc"), $str);
    }

    while (ereg("%PRODUCT_OPTION_6_DESC\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_6_DESC(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "option6desc"), $str);
    }

    while (ereg("%PRODUCT_OPTION_7_DESC\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_7_DESC(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "option7desc"), $str);
    }

    while (ereg("%PRODUCT_OPTION_8_DESC\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_8_DESC(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "option8desc"), $str);
    }

    while (ereg("%PRODUCT_OPTION_9_DESC\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_9_DESC(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "option9desc"), $str);
    }

    while (ereg("%PRODUCT_OPTION_10_DESC\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_OPTION_10_DESC(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "option10desc"), $str);
    }

    while (ereg("%PRODUCT_DESC_SHORT\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_DESC_SHORT(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "desc_short"), $str);
    }

    while (ereg("%PRODUCT_DESC_LONG1\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_DESC_LONG1(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "desc_long1"), $str);
    }

    while (ereg("%PRODUCT_DESC_LONG2\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_DESC_LONG2(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "desc_long2"), $str);
    }

    while (ereg("%PRODUCT_IMG_SMALL\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_IMG_SMALL(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "img_small"), $str);
    }

    while (ereg("%PRODUCT_IMG_MEDIUM\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_IMG_MEDIUM(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "img_medium"), $str);
    }

    while (ereg("%PRODUCT_IMG_LARGE\(([[:alpha:][:digit:][:space:]-]*)\)%", $str, $regs)) {
      $str2 = "%PRODUCT_IMG_LARGE(" . $regs[1] . ")%";
      $str = str_replace($str2, get_pdynattr_by_sku($regs[1], "img_large"), $str);
    }

    return $str;
  }

  function parse_basket_bundle ($bundle, $inner_loop) {
    global $sock;
    global $config;

    $result_array = split("(%BEGIN_INNER_LOOP%|%END_INNER_LOOP%)",$inner_loop);

    for ($a = 0; $a < count($bundle->items); $a++) {
      $item = new Item(0, $bundle->items[$a]->sku);
      $bundle_price += $bundle->items[$a]->quantity * $item->sale_price;
      $bundle_weight += $bundle->items[$a]->quantity * $item->weight;
      $result .= parse_dynamic_page($result_array[1], 0, $item->sku, 0);
    }

    $result = $result_array[0] . $result . $result_array[2];

    $result = str_replace("%BUNDLE_ID%", $bundle->id, $result);
    $result = str_replace("%BUNDLE_NAME%", $bundle->name, $result);
    $result = str_replace("%BUNDLE_QUANTITY%", $bundle->quantity, $result);
    $result = str_replace("%BUNDLE_NUM_ITEMS%", count($bundle->items), $result);
    $result = str_replace("%BUNDLE_NUM_ITEMS_PLUS1%", count($bundle->items) + 1, $result);
    $result = str_replace("%BUNDLE_PRICE%", format_money($bundle_price), $result);
    $result = str_replace("%BUNDLE_SUBTOTAL%", format_money($bundle_price * $bundle->quantity), $result);
    $result = str_replace("%BUNDLE_WEIGHT%", $bundle_weight, $result);
    $result = str_replace("%BUNDLE_TOTAL_WEIGHT%", $bundle_weight * $bundle->quantity, $result);

    return $result;
  }

  function dehtmlspecialchars($str) {
    $str = str_replace("&gt;", ">", $str);
    $str = str_replace("&lt;", "<", $str);
    $str = str_replace("&quot;", "\"", $str);
    $str = str_replace("$amp;", "&", $str);
    return $str;
  }

  function get_shiptypes_by_zone($zone_id) {
    global $sock;

    $query = "SELECT * FROM shipping_type_assignments
              WHERE zone_id = $zone_id
              ORDER BY type_id";

    if (!$sql_result = mysql_query($query, $sock)) {
      echo mysql_error();
      html_exit();
    }

    for ($a = 0; $a < mysql_num_rows($sql_result); $a++) {
      $shiptypeassign = new ShippingTypeAssignment(mysql_fetch_row($sql_result));
      $shiptype = new ShippingType(0, $shiptypeassign->type_id);
      $result[] = $shiptype;
    }

    return $result;
  }

  function get_shipcost_weight($type_id, $zone_id, $weight) {
    global $sock;

    $query = "SELECT cost FROM shipping_weight_threshold
              WHERE zone_id = $zone_id
                AND type_id = $type_id
                AND threshold >= $weight
              ORDER BY threshold";

    if (!$sql_result = mysql_query($query, $sock)) {
      echo mysql_error();
      html_exit();
    }

    if (mysql_num_rows($sql_result)) {
      $sql_result_row = mysql_fetch_row($sql_result);
      $result = $sql_result_row[0];
    }

    return $result;
  }

  function get_shipcost_price($type_id, $zone_id, $price) {
    global $sock;

    $query = "SELECT cost FROM shipping_price_threshold
              WHERE zone_id = $zone_id
                AND type_id = $type_id
                AND threshold >= $price
              ORDER BY threshold";

    if (!$sql_result = mysql_query($query, $sock)) {
      echo mysql_error();
      html_exit();
    }

    if (mysql_num_rows($sql_result)) {
      $sql_result_row = mysql_fetch_row($sql_result);
      $result = $sql_result_row[0];
    }

    return $result;
  }

  function get_shipcost_quantity($type_id, $zone_id, $quantity) {
    global $sock;

    $query = "SELECT cost FROM shipping_quantity_threshold
              WHERE zone_id = $zone_id
                AND type_id = $type_id
                AND threshold >= $quantity
              ORDER BY threshold";

    if (!$sql_result = mysql_query($query, $sock)) {
      echo mysql_error();
      html_exit();
    }

    if (mysql_num_rows($sql_result)) {
      $sql_result_row = mysql_fetch_row($sql_result);
      $result = $sql_result_row[0];
    }

    return $result;
  }

  // List the provinces in a <option> type format
  function list_provinces_select ($selected) {
    global $sock;
    global $PROVINCES;

    for ($x = 0; $x < count($PROVINCES); $x++) {
      $result .= "<option value=\"" .  $PROVINCES[$x][0];
      $result .= "\"" .  ($PROVINCES[$x][0] == $selected ? " selected" : "");
      $result .= ">" . $PROVINCES[$x][1];
    }

    return $result;
  }

  // List the states in a <option> type format
  function list_states_select ($selected) {
    global $sock;
    global $STATES;

    for ($x = 0; $x < count($STATES); $x++) {
      $result .= "<option value=\"" .  $STATES[$x][0];
      $result .= "\"" .  ($STATES[$x][0] == $selected ? " selected" : "");
      $result .= ">" . $STATES[$x][1];
    }

    return $result;
  }

  // List the provinces in a <option> type format
  function list_provinces_code_select ($selected) {
    global $sock;
    global $PROVINCES;

    for ($x = 0; $x < count($PROVINCES); $x++) {
      $result .= "<option value=\"" .  $PROVINCES[$x][0];
      $result .= "\"" .  ($PROVINCES[$x][0] == $selected ? " selected" : "");
      $result .= ">" . $PROVINCES[$x][0];
    }

    return $result;
  }

  // List the states in a <option> type format
  function list_states_code_select ($selected) {
    global $sock;
    global $STATES;

    for ($x = 0; $x < count($STATES); $x++) {
      $result .= "<option value=\"" .  $STATES[$x][0];
      $result .= "\"" .  ($STATES[$x][0] == $selected ? " selected" : "");
      $result .= ">" . $STATES[$x][0];
    }

    return $result;
  }

  // List the countries in a <option> type format
  function list_countries_select ($selected) {
    global $sock;
    global $COUNTRIES;

    for ($x = 0; $x < count($COUNTRIES); $x++) {
      $result .= "<option value=\"" .  $COUNTRIES[$x];
      $result .= "\"" .  ($COUNTRIES[$x] == $selected ? " selected" : "");
      $result .= ">" . $COUNTRIES[$x];
    }

    return $result;
  }
?>
