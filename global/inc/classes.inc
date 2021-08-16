<?php
  class Config {
    var $store_name;
    var $store_address_1;
    var $store_address_2;
    var $store_city;
    var $store_state;
    var $store_country;
    var $store_zip;
    var $store_phone;
    var $store_fax;
    var $store_url;
    var $domain_name;
    var $billing_email;
    var $shipping_origin;
    var $checkout_url;
    var $viewbasket_url;
    var $template_url;
    var $link_basket_items;
    var $tax_pst_rate;
    var $tax_gst_rate;
    var $cc_live;
    var $cc_terminal_id;
    var $pgp_use;
    var $pgp_rcptkey_id;
    var $email_sender_name;
    var $email_sender_address;
    var $email_cust_header;
    var $email_cust_footer;
    var $shipping_model;

    function Config () {
      global $sock;

      $query = "SELECT * FROM config";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $sql_result_row = mysql_fetch_row($sql_result);

      $this->store_name = stripslashes($sql_result_row[0]);
      $this->store_address_1 = stripslashes($sql_result_row[1]);
      $this->store_address_2 = stripslashes($sql_result_row[2]);
      $this->store_city = stripslashes($sql_result_row[3]);
      $this->store_state = stripslashes($sql_result_row[4]);
      $this->store_country = stripslashes($sql_result_row[5]);
      $this->store_zip = stripslashes($sql_result_row[6]);
      $this->store_phone = stripslashes($sql_result_row[7]);
      $this->store_fax = stripslashes($sql_result_row[8]);
      $this->store_url = stripslashes($sql_result_row[9]);
      $this->domain_name = stripslashes($sql_result_row[10]);
      $this->billing_email = stripslashes($sql_result_row[11]);
      $this->shipping_origin = stripslashes($sql_result_row[12]);
      $this->checkout_url = stripslashes($sql_result_row[13]);
      $this->viewbasket_url = stripslashes($sql_result_row[14]);
      $this->template_url = stripslashes($sql_result_row[15]);
      $this->link_basket_items = $sql_result_row[16];
      $this->tax_pst_rate = stripslashes($sql_result_row[17]);
      $this->tax_gst_rate = stripslashes($sql_result_row[18]);
      $this->cc_live = $sql_result_row[19];
      $this->cc_terminal_id = stripslashes($sql_result_row[20]);
      $this->pgp_use = $sql_result_row[21];
      $this->pgp_rcptkey_id = stripslashes($sql_result_row[22]);
      $this->email_sender_name = stripslashes($sql_result_row[23]);
      $this->email_sender_address = stripslashes($sql_result_row[24]);
      $this->email_cust_header = stripslashes($sql_result_row[25]);
      $this->email_cust_footer = stripslashes($sql_result_row[26]);
      $this->shipping_model = $sql_result_row[27];

      $url = parse_url($this->store_url);
      $host_array = explode(".", $url["host"]);
      $this->domain_name = $host_array[count($host_array) - 2] . "." .
                           $host_array[count($host_array) - 1];
    }

    function update () {
      global $sock;

      $query = "UPDATE config
                SET store_name = '" . addslashes($this->store_name) . "',
                store_address_1 = '" . addslashes($this->store_address_1) . "',
                store_address_2 = '" . addslashes($this->store_address_2) . "',
                store_city = '" . addslashes($this->store_city) . "',
                store_state = '" . addslashes($this->store_state) . "',
                store_country = '" . addslashes($this->store_country) . "',
                store_zip = '" . addslashes($this->store_zip) . "',
                store_phone = '" . addslashes($this->store_phone) . "',
                store_fax = '" . addslashes($this->store_fax) . "',
                store_url = '" . addslashes($this->store_url) . "',
                domain_name = '" . addslashes($this->domain_name) . "',
                billing_email = '" . addslashes($this->billing_email) . "',
                shipping_origin = '" . addslashes($this->shipping_origin) . "',
                checkout_url = '" . addslashes($this->checkout_url) . "',
                viewbasket_url = '" . addslashes($this->viewbasket_url) . "',
                template_url = '" . addslashes($this->template_url) . "',
                link_basket_items = $this->link_basket_items,
                tax_pst_rate = '" . addslashes($this->tax_pst_rate) . "',
                tax_gst_rate = '" . addslashes($this->tax_gst_rate) . "',
                cc_live = $this->cc_live,
                cc_terminal_id = '" . addslashes($this->cc_terminal_id) . "',
                pgp_use = $this->pgp_use,
                pgp_rcptkey_id = '" . addslashes($this->pgp_rcptkey_id) . "',
                email_sender_name = '" . addslashes($this->email_sender_name) . "',
                email_sender_address = '" . addslashes($this->email_sender_address) . "',
                email_cust_header = '" . addslashes($this->email_cust_header) . "',
                email_cust_footer = '" . addslashes($this->email_cust_footer) . "',
                shipping_model = $this->shipping_model";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }

  class Category {
    var $id;
    var $parent_id;
    var $template_id;
    var $name;
    var $desc_short;
    var $desc_long1;
    var $desc_long2;
    var $img_small;
    var $img_medium;
    var $img_large;
    var $num_items;

    function Category ($sql_result_row, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM categories
                  WHERE id = " . $id;

        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }

        if (mysql_num_rows($sql_result) == 1) {
          $sql_result_row = mysql_fetch_row($sql_result);
        }
      }

      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->parent_id = $sql_result_row[1];
        $this->template_id = $sql_result_row[2];
        $this->name = stripslashes($sql_result_row[3]);
        $this->desc_short = stripslashes($sql_result_row[4]);
        $this->desc_long1 = stripslashes($sql_result_row[5]);
        $this->desc_long2 = stripslashes($sql_result_row[6]);
        $this->img_small = stripslashes($sql_result_row[7]);
        $this->img_medium = stripslashes($sql_result_row[8]);
        $this->img_large = stripslashes($sql_result_row[9]);

        $query = "SELECT COUNT(*) FROM items
                  WHERE category = $this->id";

        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }

        $sql_result_row = mysql_fetch_row($sql_result);

        $this->num_items = $sql_result_row[0];
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO categories
                VALUES (NULL, $this->parent_id, $this->template_id,
                        '" . addslashes($this->name) . "',
                        '" . addslashes($this->desc_short) . "',
                        '" . addslashes($this->desc_long1) . "',
                        '" . addslashes($this->desc_long2) . "',
                        '" . addslashes($this->img_small) . "',
                        '" . addslashes($this->img_medium) . "',
                        '" . addslashes($this->img_large) . "')";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $this->id = mysql_insert_id();
    }

    function update () {
      global $sock;

      $query = "UPDATE categories
                SET parent_id = $this->parent_id,
                    template_id = $this->template_id,
                    name = '" . addslashes($this->name) . "',
                    desc_short = '" . addslashes($this->desc_short) . "',
                    desc_long1 = '" . addslashes($this->desc_long1) . "',
                    desc_long2 = '" . addslashes($this->desc_long2) . "',
                    img_small = '" . addslashes($this->img_small) . "',
                    img_medium = '" . addslashes($this->img_medium) . "',
                    img_large = '" . addslashes($this->img_large) . "'
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      $query = "DELETE FROM categories
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function get_subs () {
      global $sock;

      $query = "SELECT * FROM categories
                WHERE parent_id = $this->id
                ORDER BY name";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      for ($i = 0; $i < mysql_num_rows($sql_result); $i++) {
        $category = new Category(mysql_fetch_row($sql_result), 0);
        $result[] = $category;

        $temp = $category->get_subs();

        if ($temp) {
          for ($j = 0; $j < count($temp); $j++)
            $result[] = $temp[$j];
        }
      }

      return $result;
    }
  }

  class Item {
    var $id;
    var $category;
    var $sku;
    var $name;
    var $description;
    var $price;
    var $option1;
    var $option2;
    var $option3;
    var $option4;
    var $option5;
    var $option6;
    var $option7;
    var $option8;
    var $option9;
    var $option10;
    var $sale_price;

    function Item ($sql_result_row, $sku) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($sku) {
        $query = "SELECT * FROM items
                  WHERE sku = '" . addslashes($sku) . "'";

        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }

        if (mysql_num_rows($sql_result) == 1) {
          $sql_result_row = mysql_fetch_row($sql_result);
        }
      }

      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->category = $sql_result_row[1];
        $this->sku = stripslashes($sql_result_row[2]);
        $this->name = stripslashes($sql_result_row[3]);
        $this->price = $sql_result_row[4];
        $this->pst = $sql_result_row[5];
        $this->gst = $sql_result_row[6];
        $this->weight = $sql_result_row[7];
        $this->weight_type = $sql_result_row[8];
        $this->option1 = stripslashes($sql_result_row[9]);
        $this->option2 = stripslashes($sql_result_row[10]);
        $this->option3 = stripslashes($sql_result_row[11]);
        $this->option4 = stripslashes($sql_result_row[12]);
        $this->option5 = stripslashes($sql_result_row[13]);
        $this->option6 = stripslashes($sql_result_row[14]);
        $this->option7 = stripslashes($sql_result_row[15]);
        $this->option8 = stripslashes($sql_result_row[16]);
        $this->option9 = stripslashes($sql_result_row[17]);
        $this->option10 = stripslashes($sql_result_row[18]);

        $this->sale_price = $this->get_sale_price(0);
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO items
                VALUES (" . ($this->id ? $this->id : "NULL") . ",
                       $this->category,
                       '" .  addslashes($this->sku) . "',
                       '" .  addslashes($this->name) . "',
                       $this->price, $this->pst, $this->gst,
                       $this->weight, $this->weight_type,
                       '" . addslashes($this->option1) . "',
                       '" . addslashes($this->option2) . "',
                       '" . addslashes($this->option3) . "',
                       '" . addslashes($this->option4) . "',
                       '" . addslashes($this->option5) . "',
                       '" . addslashes($this->option6) . "',
                       '" . addslashes($this->option7) . "',
                       '" . addslashes($this->option8) . "',
                       '" . addslashes($this->option9) . "',
                       '" . addslashes($this->option10) . "')";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      if (!$this->id)
        $this->id = mysql_insert_id();
    }

    function update () {
      global $sock;

      $query = "UPDATE items
                SET id = $this->id,
                    category = $this->category,
                    sku = '" .  addslashes($this->sku) . "',
                    name = '" .  addslashes($this->name) . "',
                    price = $this->price,
                    pst = $this->pst,
                    gst = $this->gst,
                    weight = $this->weight,
                    weight_type = $this->weight_type,
                    option1 = '" . addslashes($this->option1) . "',
                    option2 = '" . addslashes($this->option2) . "',
                    option3 = '" . addslashes($this->option3) . "',
                    option4 = '" . addslashes($this->option4) . "',
                    option5 = '" . addslashes($this->option5) . "',
                    option6 = '" . addslashes($this->option6) . "',
                    option7 = '" . addslashes($this->option7) . "',
                    option8 = '" . addslashes($this->option8) . "',
                    option9 = '" . addslashes($this->option9) . "',
                    option10 = '" . addslashes($this->option10) . "'
                WHERE sku = '" . addslashes($this->sku) . "'";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      $query = "DELETE FROM items
                WHERE sku = '" . addslashes($this->sku) . "'";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function get_sale_price() {
      global $sock;

      $result = $this->price;

      $query = "SELECT * FROM discount_assignments
                WHERE sku = '" . addslashes($this->sku) . "'";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        exit();
      }

      for ($b = 0; $b < mysql_num_rows($sql_result); $b++) {
        $da = new DiscountAssignment(mysql_fetch_row($sql_result), 0);
        $dc = new DiscountCode(0, $da->code_id);
        $sc = new ScaleCode(0, $dc->code_id);

        $now = time();

        if (($dc->start_time <= $now) && 
            ($now < $dc->end_time) && ($dc->units == 0)) {

          $query = "SELECT * FROM scale_ranges
                    WHERE code_id = " . $sc->id . "
                    ORDER BY threshold";

          if (!$sql_result2 = mysql_query($query, $sock)) {
            echo mysql_error();
            exit();
          }

          if (mysql_num_rows($sql_result2)) {
            // Just look at the first threshold, since that's the
            // one that works for all items (ie, quantity 0 through
            // this threshold)
            $sr = new ScaleRange(mysql_fetch_row($sql_result2), 0);
 
            // Only count this as a sale if the value is a percentage
            // off each item or a dollar value off each item.
            if ($sr->value > 0) {
              if ($sr->type == 1)
                $result -= $sr->value;
              elseif ($sr->type == 4)
                $result -= $this->price * ($sr->value / 100);
            }
          }
        }
      }

      // If the sale dropped the price below 0, make the
      // the sale price 0.00
      if ($result < 0)
        $result = 0;

      return $result;
    }
  }

  class Order {
    var $id;
    var $basket_id;
    var $expiry;
    var $bundles;
    var $itemlist;
    var $weight;
    var $pst;
    var $gst;
    var $cost;
    var $discount;
    var $expiry_time;

    function Order () {
      $this->bundles = array();
      $this->itemlist = array();
      return;
    }

    function get_by_basket_id ($basket_id) {
      global $sock;

      $query = "SELECT id, basket_id, expiry
                FROM orders
                WHERE basket_id = '$basket_id'";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        exit();
      }

      $sql_result_row = mysql_fetch_row($sql_result);
      $this->id = $sql_result_row[0];
      $this->basket_id = $sql_result_row[1];
      $this->expiry = $sql_result_row[2];

      $query = "SELECT id, name, quantity
                FROM bundles
                WHERE order_id = $this->id
                ORDER BY id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        exit();
      }

      for ($i = 0; $i < mysql_num_rows($sql_result); $i++) {
        $sql_result_row = mysql_fetch_row($sql_result);
        $bundle = new Bundle();
        $bundle->id = $sql_result_row[0];
        $bundle->name = stripslashes($sql_result_row[1]);
        $bundle->quantity = $sql_result_row[2];

        $query = "SELECT a.id, a.sku, a.quantity,
                         b.name, b.price, b.pst, b.gst,
                         b.weight, b.weight_type
                  FROM bundle_items AS a, items AS b
                  WHERE a.sku = b.sku
                    AND a.bundle_id = $bundle->id
                  ORDER BY a.id";

        if (!$sql_result2 = mysql_query($query, $sock)) {
          echo mysql_error();
          exit();
        }

        for ($j = 0; $j < mysql_num_rows($sql_result2); $j++) {
          $sql_result_row2 = mysql_fetch_row($sql_result2);
          $bundle_item = new Bundle_Item();
          $bundle_item->id = $sql_result_row2[0];
          $bundle_item->sku = stripslashes($sql_result_row2[1]);
          $bundle_item->quantity = $sql_result_row2[2];
          $bundle_item->name = stripslashes($sql_result_row2[3]);
          $bundle_item->price = $sql_result_row2[4];
          $bundle_item->pst = $sql_result_row2[5];
          $bundle_item->gst = $sql_result_row2[6];
          $bundle_item->weight = $sql_result_row2[7];
          $bundle_item->weight_type = $sql_result_row2[8];
          $bundle->items[] = $bundle_item;

          // Add this item's weight to the order's total weight
          if ($bundle_item->weight_type == "0")
            $this->weight += $bundle_item->weight * $bundle_item->quantity * $bundle->quantity;
          elseif ($bundle_item->weight_type == "1")
            $this->weight += $bundle_item->weight * ((float) (1 / 2.2)) * $bundle_item->quantity * $bundle->quantity;


          $found = 0;

          for ($a = 0; $a < count($this->itemlist); $a++) {
            if ($this->itemlist[$a][0] == $bundle_item->sku) {
              $found = 1;
              break;
            }
          }

          if ($found) {
            $this->itemlist[$a][1] += $bundle_item->quantity;
          }
          else {
            $new_item = array($bundle_item->sku, 
                              $bundle_item->quantity * $bundle->quantity);
            $this->itemlist[] = $new_item;
          }
        }

        $this->bundles[] = $bundle;
      }

      $this->calc_sales($sock);
      $this->discount = $this->calc_discount();

      // Create the expiry_time string
      $expiry_time = $this->expiry - time();
      $expiry_hrs = (int) ($expiry_time / 3600);
      $expiry_mins = (int) (($expiry_time % 3600) / 60);

      if ($expiry_hrs)
        $order_expiry_time = "$expiry_hrs hour" . ($expiry_hrs == 1 ? "" : "s");

      if ($expiry_mins) {
        if ($expiry_hrs)
          $order_expiry_time .= " and ";

        $order_expiry_time .= "$expiry_mins minute" .
                              ($expiry_mins == 1 ? "" : "s");
      }

      if (!$expiry_hrs && !$expiry_mins)
        $order_expiry_time = "less than 1 minute";

      $this->expiry_time = $order_expiry_time;
    }

    function calc_sales() {
      global $sock;
      global $config;

      $pstable = 0;
      $gstable = 0;

      for ($i = 0; $i < count($this->bundles); $i++) {
        $bundle = $this->bundles[$i];
        $change = 0;

        for ($j = 0; $j < count($bundle->items); $j++) {
          $bundle_item = $bundle->items[$j];

          $db_item = new Item(0, $bundle_item->sku);

          if ($db_item->sale_price != $bundle_item->price) {
            $bundle_item->price = $db_item->sale_price;
            $change = 1;
          }

          if ($change)
            $bundle->items[$j] = $bundle_item;

          // Since this function is called when an order is
          // extracted from the db, we calculate the total cost
          // of the order here.
          $this->cost += $bundle_item->price * 
                         $bundle_item->quantity * $bundle->quantity;

          // If PST is to be paid on this item, add the item's cost
          // to the bundle's PST total
          if ($bundle_item->pst)
            $pstable += $bundle_item->price * 
                               $bundle_item->quantity * $bundle->quantity;

          // If GST is to be paid on this item, add the item's cost
          // to the bundle's GST total
          if ($bundle_item->gst)
            $gstable += $bundle_item->price * 
                               $bundle_item->quantity * $bundle->quantity;
        }
        if ($change)
          $this->bundles[$i] = $bundle;
      }

      $this->pst = $pstable * $config->tax_pst_rate;
      $this->gst = $gstable * $config->tax_gst_rate;
    }

    function calc_discount() {
      global $sock;

      $discount = 0;

      for ($a = 0; $a < count($this->itemlist); $a++) {
        $item = $this->itemlist[$a];
        $db_item = new Item(0, $item[0]);

        if ($db_item->sale_price != $db_item->price)
          $db_item->price = $db_item->sale_price;

        $query = "SELECT * FROM discount_assignments
                  WHERE sku = '" . addslashes($item[0]) . "'";

        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          exit();
        }

        for ($b = 0; $b < mysql_num_rows($sql_result); $b++) {
          $da = new DiscountAssignment(mysql_fetch_row($sql_result), 0);
          $dc = new DiscountCode(0, $da->code_id);
          $sc = new ScaleCode(0, $dc->code_id);

          $now = time();

          if (($dc->start_time <= $now) && ($now < $dc->end_time)) {
            $query = "SELECT * FROM scale_ranges
                      WHERE code_id = " . $sc->id;

            // Select the range, but based on the appropriate
            // measure set in the discount code
            if ($dc->units == 0)
              $limit = $item[1];
            elseif ($dc->units == 1)
              $limit = $this->weight;
            elseif ($dc->units == 2)
              $limit = $this->cost;

            $query .= " ORDER BY threshold";

            if (!$sql_result2 = mysql_query($query, $sock)) {
              echo mysql_error();
              exit();
            }

            if (mysql_num_rows($sql_result2)) {
              $last_sr = 0;

              for ($c = 0; $c < mysql_num_rows($sql_result2); $c++) {
                $sr = new ScaleRange(mysql_fetch_row($sql_result2), 0);
                $range = $sr->threshold - $last_sr->threshold;

                // Make sure that this discount isn't one that
                // was already accounted for by the automatic 
                // sale price system in get_sale_price()
                if (($c != 0) || ($sr->value <= 0) ||
                    ($sr->type != 1 && $sr->type != 4)) {

                  if ($limit > $sr->threshold)
                    $in_this_range = $range;
                  else
                    $in_this_range = $limit - $last_sr->threshold;

                  if ($in_this_range > 0) {
                    if ($sr->threshold >= $limit) {
                      if ($sr->type == 0)
                        $discount += $sr->value;
                      elseif ($sr->type == 1)
                        $discount += $sr->value * $item[1];
                      elseif ($sr->type == 3)
                        $discount += ($sr->value / 100) * $this->cost;
                      elseif ($sr->type == 4)
                        $discount += ($sr->value / 100) * 
                                     $db_item->price * $item[1];
                    }

                    if ($sr->type == 2)
                      $discount += $sr->value * $in_this_range;
                    elseif ($sr->type == 5)
                      $discount += ($sr->value / 100) * 
                                   $db_item->price * $in_this_range;
                  }

                  $last_sr = $sr;
                }
              }
            }
          }
        }
      }

      return $discount;
    }

    function add_bundle($new_bundle, $quantity) {
      global $sock;

      // For every bundle in the user's basket, check to see if the
      // the bundle being added is equal to that bundle or not by
      // first, building the bundles from the db, then comparing
      // that bundle to the one supplied in the parameters

      $equal = 0;

      // For each bundle currently in this order...
      for ($i = 0; $i < count($this->bundles); $i++) {
        $bundle = $this->bundles[$i];

        // Do the actual comparison
        if (count($bundle->items) == count($new_bundle->items)) {
          for ($j = 0; $j < count($new_bundle->items); $j++) {
            $found = 0;
            for ($k = 0; $k < count($bundle->items); $k++) {
              $new_item = $new_bundle->items[$j];
              $item = $bundle->items[$k];
              if ($new_item->sku == $item->sku) {
                $found = 1;
                break;
              }
            }
            if ($found)
              $equal = 1;
            else {
              $equal = 0;
              break;
            }
          }
        }

        if ($equal)
          break;
      }

      // If the new bundle isn't in the order already, add it.
      if (!$equal) {
        $new_bundle->quantity = $quantity;
        $this->bundles[] = $new_bundle;
      }
      // Otherwise, increment the existing bundle.
      else {
        $bundle->quantity += $quantity;
        $this->bundles[$i] = $bundle;
      }
    }

    function display () {
      echo "Your order contains " . count($this->bundles) . " bundles:<p>\n";
      echo "<ol>\n";

      for ($i = 0; $i < count($this->bundles); $i++) {
        $bundle = $this->bundles[$i];
        echo "<li>$bundle->name<br>id = $bundle->id<br>quantity = $bundle->quantity<p>Items:<p>\n";
        echo "  <ol>\n";

        for ($j = 0; $j < count($bundle->items); $j++) {
          $bundle_item = $bundle->items[$j];
          printf("<li>name = %s<br>id = %d<br>sku = %s<br>quantity = %d<br>price = \$%s<br>pst = %d<br>gst = %d<br>weight = %f<br>weight_type = %d\n",
            $bundle_item->name,
            $bundle_item->id,
            $bundle_item->sku,
            $bundle_item->quantity,
            format_money($bundle_item->price),
            $bundle_item->pst,
            $bundle_item->gst,
            $bundle_item->weight,
            $bundle_item->weight_type);
        }   
        echo "</ol>\n";
      }     
      echo "</ol>\n";
    }

    function update () {
      global $sock;

      //echo "deleting order<br>";
      $this->delete($sock);
      //echo "inserting order<br>";
      $this->insert($sock);
    }

    function insert () {
      global $sock;

      $not_empty = 0;

      // We need to make sure that the order is not
      // empty before we add any information to the db. Do
      // this by checking if there is at least one bundle 
      // with a quantity > 0, that contains at least 1 item
      // with a quantity > 0.
      for ($j = 0; $j < count($this->bundles); $j++) {
        $bundle = $this->bundles[$j];

        if ($bundle->quantity > 0) {
          for ($k = 0; $k < count($bundle->items); $k++) {
            $bundle_item = $bundle->items[$k];
            if ($bundle_item->quantity > 0) {
              $not_empty = 1;
              break;
            }
          }
        }
        if ($not_empty)
          break;
      }

      // If the order's not empty, then add it to the db
      if ($not_empty) {
        $query = "INSERT INTO orders
                  VALUES ($this->id, '$this->basket_id', $this->expiry)";
                             
        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          exit();
        }

        // For each bundle in the order...
        for ($i = 0; $i < count($this->bundles); $i++) {
          $bundle = $this->bundles[$i];

          // If the bundle has a quantity above 0...
          if ($bundle->quantity > 0) {
            $not_empty = 0;
            
            // Check to see that the bundle isn't empty, since
            // the bundle quantity might be > 0, but the bundle
            // might be empty.
            for ($j = 0; $j < count($bundle->items); $j++) {
              $bundle_item = $bundle->items[$j];
              if ($bundle_item->quantity > 0) {
                $not_empty = 1;
                break;
              }
            }

            if ($not_empty) {
              $query = "INSERT INTO bundles
                        VALUES (NULL, $this->id, '" .
                                addslashes($bundle->name) . "', 
                                $bundle->quantity)";
                             
              if (!$sql_result = mysql_query($query, $sock)) {
                echo mysql_error();
                exit();
              }

              $bundle->id = mysql_insert_id($sock);

              for ($j = 0; $j < count($bundle->items); $j++) {
                $bundle_item = $bundle->items[$j];

                $query = "INSERT INTO bundle_items
                          VALUES (NULL, $bundle->id,
                                  '$bundle_item->sku', $bundle_item->quantity)";
                             
                if (!$sql_result = mysql_query($query, $sock)) {
                  echo mysql_error();
                  exit();
                }
              } // End for each bundle item
            } // End if bundle not_empty
          } // End if bundle quantity > 0
        } // End for each bundle
      } // End if order not_empty)
    } // End function

    function delete () {
      global $sock;

      $query = "SELECT id FROM bundles
                WHERE order_id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        exit();
      }

      for ($i = 0; $i < mysql_num_rows($sql_result); $i++) {
        $sql_result_row = mysql_fetch_row($sql_result);

        $query = "DELETE FROM bundle_items
                  WHERE bundle_id = $sql_result_row[0]";

        if (!$sql_result2 = mysql_query($query, $sock)) {
          echo mysql_error();
          exit();
        }

        $query = "DELETE FROM bundles
                  WHERE id = $sql_result_row[0]";

        if (!$sql_result2 = mysql_query($query, $sock)) {
          echo mysql_error();
          exit();
        }
      }
                
      // Delete the existing order
      $query = "DELETE FROM orders
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        exit();
      }
    }
  }

  class Bundle {
    var $id;
    var $name;
    var $quantity;
    var $items;

    function Bundle () {
      $this->items = array();
      return;
    }

    function add_item($sku, $quantity) {
      global $sock;

      $found = 0;

      for ($j = 0; $j < count($this->items); $j++) {
        $bundle_item = $this->items[$j];

        // If the item already exists in this bundle,
        // update its quantity; otherwise, add the item
        // to the bundle
        if ($bundle_item->sku == $sku) {
          $bundle_item->quantity += $quantity;
          $this->items[$j] = $bundle_item;
          $found = 1;
        }
      }
      if (!$found) {
        $item = new Item(0, $sku);

        $bundle_item = new Bundle_Item();
        //$bundle_item->id = next_seq("bundle_items", $sock);
        $bundle_item->sku = $sku;
        $bundle_item->quantity = $quantity;
        $bundle_item->name = $item->name;
        $bundle_item->price = $item->price;
        $bundle_item->pst = $item->pst;
        $bundle_item->gst = $item->gst;
        $bundle_item->weight = $item->weight;
        $bundle_item->weight_type = $item->weight_type;
        $this->items[] = $bundle_item;
      }
    }

    function del_item($sku) {
      global $sock;

      for ($j = 0; $j < count($this->items); $j++) {
        $bundle_item = $this->items[$j];
        // If the item is found in the bundle, set
        // the array subscript to NULL
        if ($bundle_item->sku == $sku)
          $this->items[$j] = NULL;
      }
    }
  }
  
  class Bundle_Item {
    var $id;
    var $sku;
    var $name;
    var $price;
    var $tax1;
    var $tax2;
    var $weight;
    var $weight_type;
    var $quantity;

    function Bundle_Item () {
      return;
    }
  }


  class Transaction {
    var $id;
    var $timestamp;
    var $refno;
    var $authno;
    var $pridisplay;
    var $result;

    function Transaction ($sql_result_row, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM transactions
                  WHERE id = " . $id;

        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }

        if (mysql_num_rows($sql_result) == 1)
          $sql_result_row = mysql_fetch_row($sql_result);
        else
          $sql_result_row = 0;
      }

      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->timestamp = $sql_result_row[1];
        $this->refno = stripslashes($sql_result_row[2]);
        $this->authno = stripslashes($sql_result_row[3]);
        $this->pridisplay = stripslashes($sql_result_row[4]);
        $this->result = stripslashes($sql_result_row[5]);
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO transactions
                VALUES (NULL, $this->timestamp,
                        '" . addslashes($this->refno) . "',
                        '" . addslashes($this->authno) . "',
                        '" . addslashes($this->pridisplay) . "',
                        '" . addslashes($this->result) . "')";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $this->id = mysql_insert_id($sock);
    }

    function update () {
      global $sock;

      $query = "UPDATE transactions
                SET timestamp = $this->timestamp,
                    refno = '" . addslashes($this->refno) . "',
                    authno = '" . addslashes($this->authno) . "',
                    pridisplay = '" . addslashes($this->pridisplay) . "',
                    result = '" . addslashes($this->result) . "'
                WHERE id = " . $this->id;

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      $query = "DELETE FROM transactions
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }


  class Receipt {
    var $id;
    var $uid;
    var $receipt;

    function Receipt ($sql_result_row, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM receipts
                  WHERE id = " . $id;

        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }

        if (mysql_num_rows($sql_result) == 1)
          $sql_result_row = mysql_fetch_row($sql_result);
        else
          $sql_result_row = 0;
      }

      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->uid = $sql_result_row[1];
        $this->receipt = stripslashes($sql_result_row[2]);
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO receipts
                VALUES ($this->id, $this->uid,
                        '" . addslashes($this->receipt) . "')";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $this->id = mysql_insert_id($sock);
    }

    function update () {
      global $sock;

      $query = "UPDATE receipts
                SET id = $this->id, uid = $this->uid,
                    receipt = '" . addslashes($this->receipt) . "'
                WHERE id = " . $this->id;

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      $query = "DELETE FROM receipts
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }

  class ItemDyn {
    var $id;
    var $template_id;
    var $name;
    var $option1desc;
    var $option2desc;
    var $option3desc;
    var $option4desc;
    var $option5desc;
    var $option6desc;
    var $option7desc;
    var $option8desc;
    var $option9desc;
    var $option10desc;
    var $desc_short;
    var $desc_long1;
    var $desc_long2;
    var $img_small;
    var $img_medium;
    var $img_large;

    function ItemDyn ($sql_result_row, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM items_dynamic
                  WHERE id = " . $id;

        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }

        if (mysql_num_rows($sql_result) == 1)
          $sql_result_row = mysql_fetch_row($sql_result);
        else
          $sql_result_row = 0;
      }

      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->template_id = $sql_result_row[1];
        $this->name = stripslashes($sql_result_row[2]);
        $this->option1desc = stripslashes($sql_result_row[3]);
        $this->option2desc = stripslashes($sql_result_row[4]);
        $this->option3desc = stripslashes($sql_result_row[5]);
        $this->option4desc = stripslashes($sql_result_row[6]);
        $this->option5desc = stripslashes($sql_result_row[7]);
        $this->option6desc = stripslashes($sql_result_row[8]);
        $this->option7desc = stripslashes($sql_result_row[9]);
        $this->option8desc = stripslashes($sql_result_row[10]);
        $this->option9desc = stripslashes($sql_result_row[11]);
        $this->option10desc = stripslashes($sql_result_row[12]);
        $this->desc_short = stripslashes($sql_result_row[13]);
        $this->desc_long1 = stripslashes($sql_result_row[14]);
        $this->desc_long2 = stripslashes($sql_result_row[15]);
        $this->img_small = stripslashes($sql_result_row[16]);
        $this->img_medium = stripslashes($sql_result_row[17]);
        $this->img_large = stripslashes($sql_result_row[18]);
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO items_dynamic
                VALUES ($this->id, 
                        $this->template_id,
                        '" . addslashes($this->name) . "',
                        '" . addslashes($this->option1desc) . "',
                        '" . addslashes($this->option2desc) . "',
                        '" . addslashes($this->option3desc) . "',
                        '" . addslashes($this->option4desc) . "',
                        '" . addslashes($this->option5desc) . "',
                        '" . addslashes($this->option6desc) . "',
                        '" . addslashes($this->option7desc) . "',
                        '" . addslashes($this->option8desc) . "',
                        '" . addslashes($this->option9desc) . "',
                        '" . addslashes($this->option10desc) . "',
                        '" . addslashes($this->desc_short) . "',
                        '" . addslashes($this->desc_long1) . "',
                        '" . addslashes($this->desc_long2) . "',
                        '" . addslashes($this->img_small) . "',
                        '" . addslashes($this->img_medium) . "',
                        '" . addslashes($this->img_large) . "')";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function update () {
      global $sock;

      $query = "UPDATE items_dynamic
                SET template_id = " . $this->template_id . ",
                    name =  '" . addslashes($this->name) . "',
                    option1desc =  '" . addslashes($this->option1desc) . "',
                    option2desc =  '" . addslashes($this->option2desc) . "',
                    option3desc =  '" . addslashes($this->option3desc) . "',
                    option4desc =  '" . addslashes($this->option4desc) . "',
                    option5desc =  '" . addslashes($this->option5desc) . "',
                    option6desc =  '" . addslashes($this->option6desc) . "',
                    option7desc =  '" . addslashes($this->option7desc) . "',
                    option8desc =  '" . addslashes($this->option8desc) . "',
                    option9desc =  '" . addslashes($this->option9desc) . "',
                    option10desc =  '" . addslashes($this->option10desc) . "',
                    desc_short =  '" . addslashes($this->desc_short) . "',
                    desc_long1 =  '" . addslashes($this->desc_long1) . "',
                    desc_long2 =  '" . addslashes($this->desc_long2) . "',
                    img_small =  '" . addslashes($this->img_small) . "',
                    img_medium =  '" . addslashes($this->img_medium) . "',
                    img_large =  '" . addslashes($this->img_large) . "'
                WHERE id = " . $this->id;

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }

  class Template {
    var $id;
    var $type;
    var $filename;
    var $description;

    function Template ($sql_result_row, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM templates
                  WHERE id = " . $id;
        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }
        if (mysql_num_rows($sql_result) == 1)
          $sql_result_row = mysql_fetch_row($sql_result);
        else
          $sql_result_row = 0;
      }
      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->type = $sql_result_row[1];
        $this->filename = stripslashes($sql_result_row[2]);
        $this->description = stripslashes($sql_result_row[3]);
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO templates
                VALUES (NULL, $this->type,
                        '" . addslashes($this->filename) . "',
                        '" . addslashes($this->description) . "')";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $this->id = mysql_insert_id($sock);
    }

    function update () {
      global $sock;

      $query = "UPDATE templates
                SET type = $this->type,
                    filename = '" . addslashes($this->filename) . "',
                    description = '" . addslashes($this->description) . "'
                WHERE id = " . $this->id;

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      $query = "DELETE FROM templates
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }

  class ShippingType {
    var $id;
    var $description;

    function ShippingType ($sql_result_row, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM shipping_types
                  WHERE id = " . $id;
        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }
        if (mysql_num_rows($sql_result) == 1)
          $sql_result_row = mysql_fetch_row($sql_result);
        else
          $sql_result_row = 0;
      }
      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->description = stripslashes($sql_result_row[1]);
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO shipping_types
                VALUES (NULL, '" . addslashes($this->description) . "')";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $this->id = mysql_insert_id($sock);
    }

    function update () {
      global $sock;

      $query = "UPDATE shipping_types
                SET description = '" . addslashes($this->description) . "'
                WHERE id = " . $this->id;

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      // Delete the type
      $query = "DELETE FROM shipping_types
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      // Delete the zone-to-type assignments for the type
      $query = "DELETE FROM shipping_type_assignments
                WHERE type_id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      // Delete the shipping threshold info for this type
      $query = "DELETE FROM shipping_price_threshold
                WHERE type_id = $this->id";
      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
      $query = "DELETE FROM shipping_weight_threshold
                WHERE type_id = $this->id";
      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
      $query = "DELETE FROM shipping_quantity_threshold
                WHERE type_id = $this->id";
      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }

  class ShippingTypeAssignment {
    var $type_id;
    var $zone_id;

    function ShippingTypeAssignment ($sql_result_row) {
      global $sock;

      if ($sql_result_row) {
        $this->type_id = $sql_result_row[0];
        $this->zone_id = $sql_result_row[1];
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO shipping_type_assignments
                VALUES ($this->type_id, $this->zone_id)";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      $query = "DELETE FROM shipping_type_assignments
                WHERE type_id = $this->type_id
                  AND zone_id = $this->zone_id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }

  class ShippingZone {
    var $id;
    var $description;
    var $tax;
    var $tax_rate;

    function ShippingZone ($sql_result_row, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM shipping_zones
                  WHERE id = " . $id;
        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }
        if (mysql_num_rows($sql_result) == 1)
          $sql_result_row = mysql_fetch_row($sql_result);
        else
          $sql_result_row = 0;
      }
      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->description = stripslashes($sql_result_row[1]);
        $this->tax = $sql_result_row[2];
        $this->tax_rate = $sql_result_row[3];
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO shipping_zones
                VALUES (NULL, '" . addslashes($this->description) . "',
                        $this->tax, $this->tax_rate)";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $this->id = mysql_insert_id($sock);
    }

    function update () {
      global $sock;

      $query = "UPDATE shipping_zones
                SET description = '" . addslashes($this->description) . "',
                    tax = $this->tax, tax_rate = $this->tax_rate
                WHERE id = " . $this->id;

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      // Delete the zone
      $query = "DELETE FROM shipping_zones
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      // Delete the zone-to-type assignments
      $query = "DELETE FROM shipping_type_assignments
                WHERE zone_id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      // Delete the state-to-zone assignments
      $query = "DELETE FROM shipping_zone_assignments
                WHERE zone_id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      // Delete the shipping threshold info for this zone
      $query = "DELETE FROM shipping_price_threshold
                WHERE zone_id = $this->id";
      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
      $query = "DELETE FROM shipping_weight_threshold
                WHERE zone_id = $this->id";
      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
      $query = "DELETE FROM shipping_quantity_threshold
                WHERE zone_id = $this->id";
      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }

  class ShippingZoneAssignment {
    var $zone_id;
    var $state;

    function ShippingZoneAssignment ($sql_result_row, $state) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($state) {
        $query = "SELECT * FROM shipping_zone_assignments
                  WHERE state = '" . $state . "'";
        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }
        if (mysql_num_rows($sql_result) == 1)
          $sql_result_row = mysql_fetch_row($sql_result);
        else
          $sql_result_row = 0;
      }
      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->zone_id = $sql_result_row[0];
        $this->state = $sql_result_row[1];
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO shipping_zone_assignments
                VALUES ($this->zone_id, '$this->state')";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function update () {
      global $sock;

      $query = "UPDATE shipping_zone_assignments
                SET zone_id = $this->zone_id
                WHERE state = '" . $this->state . "'";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      $query = "DELETE FROM shipping_zone_assignments
                WHERE zone_id = $this->zone_id
                  AND state = '$this->state'";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }

  class ShippingPriceThreshold {
    var $id;
    var $zone_id;
    var $type_id;
    var $threshold;
    var $cost;

    function ShippingPriceThreshold ($sql_result_row, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM shipping_price_threshold
                  WHERE id = " . $id;
        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }
        if (mysql_num_rows($sql_result) == 1)
          $sql_result_row = mysql_fetch_row($sql_result);
        else
          $sql_result_row = 0;
      }
      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->zone_id = $sql_result_row[1];
        $this->type_id = $sql_result_row[2];
        $this->threshold = $sql_result_row[3];
        $this->cost = $sql_result_row[4];
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO shipping_price_threshold
                VALUES (NULL, $this->zone_id, $this->type_id,
                        $this->threshold, $this->cost)";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $this->id = mysql_insert_id($sock);
    }

    function update () {
      global $sock;

      $query = "UPDATE shipping_price_threshold
                SET zone_id = $this->zone_id, type_id = $this->type_id,
                    threshold = $this->threshold, cost = $this->cost
                WHERE id = " . $this->id;

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      $query = "DELETE FROM shipping_price_threshold
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }

  class ShippingWeightThreshold {
    var $id;
    var $zone_id;
    var $type_id;
    var $threshold;
    var $cost;

    function ShippingWeightThreshold ($sql_result_row, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM shipping_weight_threshold
                  WHERE id = " . $id;
        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }
        if (mysql_num_rows($sql_result) == 1)
          $sql_result_row = mysql_fetch_row($sql_result);
        else
          $sql_result_row = 0;
      }
      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->zone_id = $sql_result_row[1];
        $this->type_id = $sql_result_row[2];
        $this->threshold = $sql_result_row[3];
        $this->cost = $sql_result_row[4];
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO shipping_weight_threshold
                VALUES (NULL, $this->zone_id, $this->type_id,
                        $this->threshold, $this->cost)";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $this->id = mysql_insert_id($sock);
    }

    function update () {
      global $sock;

      $query = "UPDATE shipping_weight_threshold
                SET zone_id = $this->zone_id, type_id = $this->type_id,
                    threshold = $this->threshold, cost = $this->cost
                WHERE id = " . $this->id;

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      $query = "DELETE FROM shipping_weight_threshold
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }

  class ShippingQuantityThreshold {
    var $id;
    var $zone_id;
    var $type_id;
    var $threshold;
    var $cost;

    function ShippingQuantityThreshold ($sql_result_row, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM shipping_quantity_threshold
                  WHERE id = " . $id;
        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }
        if (mysql_num_rows($sql_result) == 1)
          $sql_result_row = mysql_fetch_row($sql_result);
        else
          $sql_result_row = 0;
      }
      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->zone_id = $sql_result_row[1];
        $this->type_id = $sql_result_row[2];
        $this->threshold = $sql_result_row[3];
        $this->cost = $sql_result_row[4];
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO shipping_quantity_threshold
                VALUES (NULL, $this->zone_id, $this->type_id,
                        $this->threshold, $this->cost)";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $this->id = mysql_insert_id($sock);
    }

    function update () {
      global $sock;

      $query = "UPDATE shipping_quantity_threshold
                SET zone_id = $this->zone_id, type_id = $this->type_id,
                    threshold = $this->threshold, cost = $this->cost
                WHERE id = " . $this->id;

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      $query = "DELETE FROM shipping_quantity_threshold
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }

  class ScaleCode {
    var $id;
    var $description;

    function ScaleCode ($sql_result_row, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM scale_codes
                  WHERE id = " . $id;
        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }
        if (mysql_num_rows($sql_result) == 1)
          $sql_result_row = mysql_fetch_row($sql_result);
        else
          $sql_result_row = 0;
      }
      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->description = stripslashes($sql_result_row[1]);
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO scale_codes
                VALUES (NULL, '" . addslashes($this->description) . "')";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $this->id = mysql_insert_id($sock);
    }

    function update () {
      global $sock;

      $query = "UPDATE scale_codes
                SET description = '" . addslashes($this->description) . "'
                WHERE id = " . $this->id;

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      // Delete the type
      $query = "DELETE FROM scale_codes
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      // Delete the scale ranges for this code
      $query = "DELETE FROM scale_ranges
                WHERE code_id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      // Delete the discount codes using this scale_code
      $query = "DELETE FROM discount_codes
                WHERE code_id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }

  class ScaleRange {
    var $id;
    var $code_id;
    var $threshold;
    var $value;
    var $type;

    function ScaleRange ($sql_result_row, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM scale_ranges
                  WHERE id = " . $id;
        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }
        if (mysql_num_rows($sql_result) == 1)
          $sql_result_row = mysql_fetch_row($sql_result);
        else
          $sql_result_row = 0;
      }
      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->code_id = $sql_result_row[1];
        $this->threshold = $sql_result_row[2];
        $this->value = $sql_result_row[3];
        $this->type = $sql_result_row[4];
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO scale_ranges
                VALUES (NULL, $this->code_id, $this->threshold,
                        $this->value, $this->type)";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $this->id = mysql_insert_id($sock);
    }

    function update () {
      global $sock;

      $query = "UPDATE scale_ranges
                SET code_id = $this->code_id, threshold = $this->threshold,
                    value = $this->value, type = $this->type
                WHERE id = " . $this->id;

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      $query = "DELETE FROM scale_ranges
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }

  class DiscountCode {
    var $id;
    var $description;
    var $start_time;
    var $end_time;
    var $units;
    var $code_id;

    function DiscountCode ($sql_result_row, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM discount_codes
                  WHERE id = " . $id;
        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }
        if (mysql_num_rows($sql_result) == 1)
          $sql_result_row = mysql_fetch_row($sql_result);
        else
          $sql_result_row = 0;
      }
      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->description = stripslashes($sql_result_row[1]);
        $this->start_time = $sql_result_row[2];
        $this->end_time = $sql_result_row[3];
        $this->units = $sql_result_row[4];
        $this->code_id = $sql_result_row[5];
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO discount_codes
                VALUES (NULL, '" . addslashes($this->description) . "',
                        $this->start_time, $this->end_time,
                        $this->units, $this->code_id)";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $this->id = mysql_insert_id($sock);
    }

    function update () {
      global $sock;

      $query = "UPDATE discount_codes
                SET description = '" . addslashes($this->description) . "',
                    start_time = $this->start_time, end_time = $this->end_time,
                    units = $this->units, code_id = $this->code_id
                WHERE id = " . $this->id;

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      $query = "DELETE FROM discount_codes
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $query = "DELETE FROM discount_assignments
                WHERE code_id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }

  class DiscountAssignment {
    var $id;
    var $code_id;
    var $sku;

    function DiscountAssignment ($sql_result_row, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM discount_assignments
                  WHERE id = " . $id;
        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }
        if (mysql_num_rows($sql_result) == 1)
          $sql_result_row = mysql_fetch_row($sql_result);
        else
          $sql_result_row = 0;
      }
      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->code_id = $sql_result_row[1];
        $this->sku = stripslashes($sql_result_row[2]);
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO discount_assignments
                VALUES (NULL, $this->code_id,
                        '" . addslashes($this->sku) . "')";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $this->id = mysql_insert_id($sock);
    }

    function update () {
      global $sock;

      $query = "UPDATE discount_assignments
                SET code_id = $this->code_id, 
                    description = '" . addslashes($this->sku) . "'
                WHERE id = " . $this->id;

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      $query = "DELETE FROM discount_assignments
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }

  class User {
    var $id;
    var $gid;
    var $username;
    var $password;

    function User ($sql_result_row, $username, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM users
                  WHERE id = " . $id;

        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }

        if (mysql_num_rows($sql_result) == 1) {
          $sql_result_row = mysql_fetch_row($sql_result);
        }
      }
      elseif ($username) {
        $query = "SELECT * FROM users
                  WHERE username = '" . $username . "'";

        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }

        if (mysql_num_rows($sql_result) == 1) {
          $sql_result_row = mysql_fetch_row($sql_result);
        }
      }

      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->gid = $sql_result_row[1];
        $this->username = stripslashes($sql_result_row[2]);
        $this->password = stripslashes($sql_result_row[3]);
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO users
                VALUES (NULL, $this->gid,
                        '" . addslashes($this->username) . "',
                        '" . addslashes($this->password) . "')";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $this->id = mysql_insert_id();
    }

    function update () {
      global $sock;

      $query = "UPDATE users
                SET gid = $this->gid,
                    username = '" . addslashes($this->username) . "',
                    password = '" . addslashes($this->password) . "'
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      $query = "DELETE FROM users
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $query = "DELETE FROM addressbook
                WHERE uid = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function getdefaultaddress () {
      global $sock;

      $query = "SELECT * FROM addressbook
                WHERE uid = " . $this->id . "
                ORDER BY id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $address = new Address(mysql_fetch_row($sql_result), 0);

      if ($address->id)
        return $address;
      else
        return 0;
    }

    function getaddresses () {
      global $sock;

      $query = "SELECT * FROM addressbook
                WHERE uid = " . $this->id . "
                ORDER BY id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      for ($a = 0; $a < mysql_num_rows($sql_result); $a++)
        $addresses[] = new Address(mysql_fetch_row($sql_result), 0);

      if (count($addresses))
        return $addresses;
      else
        return 0;
    }
  }

  class Address {
    var $id;
    var $uid;
    var $description;
    var $title;
    var $fname;
    var $mname;
    var $lname;
    var $company;
    var $address1;
    var $address2;
    var $address3;
    var $city;
    var $state;
    var $country;
    var $zipcode;
    var $phone1;
    var $phone2;
    var $phone3;
    var $email;
    var $url;
    var $age;
    var $gender;
    var $marital;
    var $notify;

    function Address ($sql_result_row, $id) {
      global $sock;

      // If a second argument is passed, then we know we're trying to
      // populate this object by getting the data from the database,
      // based on the id.  We'll just put the data into the
      // sql_result_row variable, so that the next paragraph picks it up.
      if ($id) {
        $query = "SELECT * FROM addressbook
                  WHERE id = " . $id;

        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          html_exit();
        }

        if (mysql_num_rows($sql_result) == 1) {
          $sql_result_row = mysql_fetch_row($sql_result);
        }
      }

      // Either 1) the sql_result_row was passed to us as a parameter,
      // or 2) the variable was set in the above paragraph. Either way,
      // populate this object's variables.
      if ($sql_result_row) {
        $this->id = $sql_result_row[0];
        $this->uid = $sql_result_row[1];
        $this->description = stripslashes($sql_result_row[2]);
        $this->title = stripslashes($sql_result_row[3]);
        $this->fname = stripslashes($sql_result_row[4]);
        $this->mname = stripslashes($sql_result_row[5]);
        $this->lname = stripslashes($sql_result_row[6]);
        $this->company = stripslashes($sql_result_row[7]);
        $this->address1 = stripslashes($sql_result_row[8]);
        $this->address2 = stripslashes($sql_result_row[9]);
        $this->address3 = stripslashes($sql_result_row[10]);
        $this->city = stripslashes($sql_result_row[11]);
        $this->state = stripslashes($sql_result_row[12]);
        $this->country = stripslashes($sql_result_row[13]);
        $this->zipcode = stripslashes($sql_result_row[14]);
        $this->phone1 = stripslashes($sql_result_row[15]);
        $this->phone2 = stripslashes($sql_result_row[16]);
        $this->phone3 = stripslashes($sql_result_row[17]);
        $this->email = stripslashes($sql_result_row[18]);
        $this->url = stripslashes($sql_result_row[19]);
        $this->age = $sql_result_row[20];
        $this->gender = stripslashes($sql_result_row[21]);
        $this->marital = stripslashes($sql_result_row[22]);
        $this->notify = $sql_result_row[23];
      }
      // No parameters, so just return, creating an empty object.
      else
        return;
    }

    function insert () {
      global $sock;

      $query = "INSERT INTO addressbook
                VALUES (NULL, $this->uid,
                        '" . addslashes($this->description) . "',
                        '" . addslashes($this->title) . "',
                        '" . addslashes($this->fname) . "',
                        '" . addslashes($this->mname) . "',
                        '" . addslashes($this->lname) . "',
                        '" . addslashes($this->company) . "',
                        '" . addslashes($this->address1) . "',
                        '" . addslashes($this->address2) . "',
                        '" . addslashes($this->address3) . "',
                        '" . addslashes($this->city) . "',
                        '" . addslashes($this->state) . "',
                        '" . addslashes($this->country) . "',
                        '" . addslashes($this->zipcode) . "',
                        '" . addslashes($this->phone1) . "',
                        '" . addslashes($this->phone2) . "',
                        '" . addslashes($this->phone3) . "',
                        '" . addslashes($this->email) . "',
                        '" . addslashes($this->url) . "',
                        $this->age,
                        '" . addslashes($this->gender) . "',
                        '" . addslashes($this->marital) . "',
                        $this->notify)";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }

      $this->id = mysql_insert_id();
    }

    function update () {
      global $sock;

      $query = "UPDATE addressbook
                SET uid = $this->uid,
                    description = '" . addslashes($this->description) . "',
                    title = '" . addslashes($this->title) . "',
                    fname = '" . addslashes($this->fname) . "',
                    mname = '" . addslashes($this->mname) . "',
                    lname = '" . addslashes($this->lname) . "',
                    company = '" . addslashes($this->company) . "',
                    address1 = '" . addslashes($this->address1) . "',
                    address2 = '" . addslashes($this->address2) . "',
                    address3 = '" . addslashes($this->address3) . "',
                    city = '" . addslashes($this->city) . "',
                    state = '" . addslashes($this->state) . "',
                    country = '" . addslashes($this->country) . "',
                    zipcode = '" . addslashes($this->zipcode) . "',
                    phone1 = '" . addslashes($this->phone1) . "',
                    phone2 = '" . addslashes($this->phone2) . "',
                    phone3 = '" . addslashes($this->phone3) . "',
                    email = '" . addslashes($this->email) . "',
                    url = '" . addslashes($this->url) . "',
                    age = $this->age,
                    gender = '" . addslashes($this->gender) . "',
                    marital = '" . addslashes($this->marital) . "',
                    notify = $this->notify
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }

    function delete () {
      global $sock;

      $query = "DELETE FROM addressbook
                WHERE id = $this->id";

      if (!$sql_result = mysql_query($query, $sock)) {
        echo mysql_error();
        html_exit();
      }
    }
  }
?>
