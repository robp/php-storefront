<?php
  // Get some configuration constants and functions
  require("inc/global_config.inc");
  require("inc/config.inc");
  require("inc/classes.inc");
  require("inc/functions.inc");
  require("inc/html.inc");

  // Connect to the SQL server and select the database
  $sock = db_connect();
  $config = new Config();

  // Setup which form inputs are required
  $required1 = array("q");

  // Check to see that all required form inputs are found
  for ($i = 0; $i < sizeof($required1); $i++) {
    if (!$$required1[$i]) {
      $missing_info = 1;
      break;
    }
  }


  if (!$missing_info) {
    $query = "SELECT id
              FROM items_dynamic
              WHERE ( ";

    $terms = explode(" ", addslashes(htmlspecialchars($q)));

    for ($a = 0; $a < count($terms); $a++) {
      if ($a) $query .= "AND ";
      $query .= "(name LIKE '%$terms[$a]%' 
                 OR desc_short LIKE '%$terms[$a]%'
                 OR desc_long1 LIKE '%$terms[$a]%'
                 OR desc_long2 LIKE '%$terms[$a]%')";
    }

    $query .= ") ORDER BY name";

    if (!$sql_result = mysql_query($query, $sock)) {
      echo mysql_error();
      html_exit();
    }

    $num_results = mysql_num_rows($sql_result);

    $tmpl_url = $config->template_url . SEARCH_RESULTS;

    $file = fopen($tmpl_url, "r");

    if ($file) {
      // Read the whole template file into $result
      while (!feof($file)) {
        $result .= fgets($file, 1024);
      }

      $result_array = split ("(%BEGIN_LOOP%|%END_LOOP%)", $result);
      $result_array[0] = str_replace("%SEARCH_KEYWORDS%", $q, $result_array[0]);
      $result_array[0] = str_replace("%SEARCH_NUM_RESULTS%", $num_results, $result_array[0]);
      $result_array[2] = str_replace("%SEARCH_KEYWORDS%", $q, $result_array[2]);
      $result_array[2] = str_replace("%SEARCH_NUM_RESULTS%", $num_results, $result_array[2]);

      $result = parse_dynamic_page($result_array[0], 0, 0, 0);

      for ($a = 1; $a <= $num_results; $a++) {
        $sql_result_row = mysql_fetch_row($sql_result);
        $itemdyn = new ItemDyn(0, $sql_result_row[0]);

        $result_loop = $result_array[1];

        // Gotta fix this ad_display crap
        //$foo = ad_display(AD_LOC_ID, 0, 1);
        //$result = str_replace("%AD_DISPLAY%", $foo);

        $result_loop = str_replace("%LOOP_ID%", $a, $result_loop);

        $result_loop = parse_dynamic_page($result_loop, $itemdyn->id, 0, 0);

        $result .= $result_loop;
      }

      $result .= parse_dynamic_page($result_array[2], 0, 0, 0);
    }
    else {
      $errmsg = "Unable to open search results template.";
    }
  }
  else {
    $errmsg = "No search keywords specified.";
  }

  if ($errmsg)
    echo $errmsg;
  else
    echo $result;
?>
