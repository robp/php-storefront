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

  // If client supplies a basket_id, check to see
  // if there's actually anything in the basket. If not,
  // delete the cookie
  if ($basket_id) {
    if (!db_item_exists("basket_id", "'$basket_id'", "orders")) {
      SetCookie("basket_id", "", 0, "", "." . $config->domain_name);
      $basket_id = 0;
    } 
  }

  if (!$referer)
    $referer = getenv("HTTP_REFERER");

  if ($basket_id) {
    // Show the shopping basket

    // Get the order
    $order = new Order();
    $order->get_by_basket_id($basket_id);

    $tmpl_url = $config->template_url . "viewbasket.html";

    $file = fopen($tmpl_url, "r");

    if ($file) {
      // Read the whole template file into $result
      while (!feof($file)) {
        $result .= fgets($file, 1024);
      }

      fclose($file);

      // Replace some things that can appear anywhere in the page, but that
      // wouldn't appear in a category or product page since we save those
      // for the bundle parsing
      $result = str_replace("%ORDER_SUBTOTAL%", 
                             format_money($order->cost), $result);
      $result = str_replace("%ORDER_WEIGHT%", $order->weight, $result);
      $result = str_replace("%ORDER_EXPIRY_TIME%", $order->expiry_time, $result);
      $result = str_replace("%HTTP_REFERER%", $referer, $result);
      $result = str_replace("%DISCOUNT_VALUE%",
                             format_money($order->discount), $result);

      $result_array = split ("(%BEGIN_DISCOUNT%|%END_DISCOUNT%)", $result);

      if (count($result_array) == 3) {
        $result = $result_array[0];
        if ($order->discount > 0) {
          $result .= $result_array[1];
        }
        $result .= $result_array[2];
      }

      $result_array = split ("(%BEGIN_LOOP%|%END_LOOP%)", $result);
      $result = parse_dynamic_page($result_array[0], 0, 0, 0);

      for ($i = 0; $i < count($order->bundles); $i++) {
        $result .= parse_basket_bundle($order->bundles[$i], $result_array[1]);
      }

      $result .= parse_dynamic_page($result_array[2], 0, 0, 0);
    }
  }
  else {
    // Show the empty basket template
    $tmpl_url = $config->template_url . "viewbasket-empty.html";

    $file = fopen($tmpl_url, "r");

    if ($file) {
      // Read the whole template file into $result
      while (!feof($file)) {
        $result .= fgets($file, 1024);
      }

      fclose($file);

      $result = parse_dynamic_page($result, 0, 0, 0);
      $result = str_replace("%HTTP_REFERER%", $referer, $result);
    }
  }

  if ($errmsg)
    echo $errmsg;
  else
    echo $result;
?>
