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

  // If the client supplies a basket_id, check to make
  // sure it exists in the database. If not, delete the
  // cookie from the client
  if ($basket_id) {
    if (!db_item_exists("basket_id", "'$basket_id'", "orders")) {
      SetCookie("basket_id", "", 0, "", "." . $config->domain_name);
      header("Location: viewbasket.phtml?referer=$referer");
      exit();
    }
  }
  else {
    if (!strlen($referer))
      $referer = $config->store_url;

    header("Location: $referer");
    exit();
  }

  $order = new Order();
  $order->get_by_basket_id($basket_id);

  // Continue shopping. Return the client to the previous
  // page (the referer).
  if ($shop || $shop_x) {
    if (!strlen($referer))
      $referer = $config->store_url;

    header("Location: $referer");
    exit();
  }
  // Proceed to the checkout page.
  elseif ($checkout || $checkout_x) {
    header("Location: " . $config->checkout_url . "?referer=$referer");
    exit();
  }
  // Remove all items from the basket. That is, delete the order.
  elseif ($empty || $empty_x) {
    $order->delete();
  }
  // This clause re-calculates an order after changes
  // have been made to the basket
  elseif ($update || $update_x) {
    for ($i = 0; $i < count($HTTP_POST_VARS); $i++) {
      $keyval = each($HTTP_POST_VARS);

      if ($keyval[0] != "referer" && 
          $keyval[0] != "update" && 
          $keyval[0] != "update_x" &&
          $keyval[0] != "update_y" &&
          $keyval[0] != "remove" && 
          (int) $keyval[1] >= 0) {

        $bundle_id = (int) $keyval[0];
        $quantity = (int) $keyval[1];

        // check if it's really in the client's order.
        for ($j = 0; $j < count($order->bundles); $j++) {
          $bundle = $order->bundles[$j];
          if ($bundle->id == $bundle_id) {
            // If the new quantity > 0, and if the bundle hasn't
            // had its quantity set to 0 by a deletion, then
            // update the quantity.
            if ($quantity > 0 && $bundle->quantity != 0)
              $bundle->quantity = $quantity;
            else
              $bundle->quantity = 0;

            $order->bundles[$j] = $bundle;
            break;
          }
        }
      }
      // If a remove array exists, such as when the designer
      // uses checkboxes to indicate bundles to be removed, then
      // remove those bundles.
      //
      // PHP has a strange way of interpreting this array:
      //
      // keyval[0] = "remove"
      // keyval[1] = array( array("1", "value1"), array("2", "value2"), etc);
      //
      // AND, I have to use each() to access each item, for some reason.
      //
      elseif ($keyval[0] == "remove") {
        // for each bundle_array in the remove[] array...
        for ($j = 0; $j < count($keyval[1]); $j++) {
          $bundle_array = each($keyval[1]);

          // check if it's really in the client's basket.
          for ($k = 0; $k < count($order->bundles); $k++) {
            $bundle = $order->bundles[$k];
            if ($bundle->id == $bundle_array[1]) {
              $bundle->quantity = 0;
              $order->bundles[$k] = $bundle;
              $bundle = $order->bundles[$k];
              break;
            }
          }
        }
      }
    }
  }
  // This clause handles the "Remove bundle" type trash-can icons
  // that remove one bundle at a time.
  elseif ($remove) {
    $bundle_id = $remove;

    // check if it's really in the clients basket.
    for ($i = 0; $i < count($order->bundles); $i++) {
      $bundle = $order->bundles[$i];
      if ($bundle->id == $bundle_id) {
        $bundle->quantity = 0;
        $order->bundles[$i] = $bundle;
        break;
      }
    }
  }

  // By setting any quantities to "0" above, they'll be removed by
  // the update() function.
  if (!$empty && !$empty_x)
    $order->update();

  $order_empty = 1;

  // Is there anything left in the basket? If not,
  // delete the cookie
  for ($j = 0; $j < count($order->bundles); $j++) {
    $bundle = $order->bundles[$j];
    if ($bundle->quantity > 0)
      $order_empty = 0;
  }

  if ($order_empty)
    SetCookie("basket_id", "", 0, "", "." . $config->domain_name);

  header("Location: viewbasket.phtml?referer=$referer");
  exit();
?>
