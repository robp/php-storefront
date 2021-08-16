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

  if ($add || $add_x)
    $redirect_url = $config->viewbasket_url;
  elseif ($buy || $buy_x)
    $redirect_url = $config->checkout_url;
  else
    $redirect_url = $config->viewbasket_url;

  // If the client supplies a basket_id, check to make
  // sure it exists in the database. If not, delete the
  // cookie from the client
  if ($basket_id) {
    if (!db_item_exists("basket_id", "'$basket_id'", "orders")) {
      SetCookie("basket_id", "", 0, "", "." . $config->domain_name);
      header("Location: $redirect_url");
      exit();
    }
    else {
      // A valid basket_id, so get the current order information
      // from the database
      $order = new Order();
      $order->get_by_basket_id($basket_id);
    }
  }
  // If there's no basket_id, create one and set
  // the cookie, and create a new Order object
  else {
    $order = new Order();
    $order->id = next_seq("orders_id");
    $order->basket_id = generate_basket_id();
    $order->expiry = time() + BASKET_ID_LIFE;
    SetCookie("basket_id", $order->basket_id, $order->expiry, 
              "", "." . $config->domain_name);
  }

  for ($h = 0; $h < MAX_BUNDLES; $h++) {
    $bundle = new Bundle();

    $quantity = $h . "-quantity";

    if (!$$quantity)
      continue;

    // Only add the order entry when a valid item
    // is found in the bundle
    $valid_bundle = 0;

    for ($i = 0; $i < MAX_BUNDLE_ITEMS; $i++) {
      $sku = $h . "-" . $i . "-sku";
      $quantity = $h . "-" . $i . "-quantity";

      if (!$$quantity)
        $$quantity = 1;

      // Is a sku passed directly?
      if ($$sku) {
        // Does the item exist in the db?
        if (db_item_exists("sku", "'" . $$sku . "'", "items")) {
          $bundle->add_item($$sku, $$quantity);
          $valid_bundle = 1;
        }
      }
      // If no sku is passed, look for an item id
      else {
        $id = $h . "-" . $i . "-id";

        // If the item id is not found, skip this number
        if (!$$id)
          continue;

        $option1 = $h . "-" . $i . "-option1";
        $option2 = $h . "-" . $i . "-option2";
        $option3 = $h . "-" . $i . "-option3";
        $option4 = $h . "-" . $i . "-option4";
        $option5 = $h . "-" . $i . "-option5";
        $option6 = $h . "-" . $i . "-option6";
        $option7 = $h . "-" . $i . "-option7";
        $option8 = $h . "-" . $i . "-option8";
        $option9 = $h . "-" . $i . "-option9";
        $option10 = $h . "-" . $i . "-option10";

        $query = "SELECT sku FROM items
                  WHERE id = " . $$id . " AND
                        option1 = '" . addslashes(dehtmlspecialchars($$option1)) . "' AND
                        option2 = '" . addslashes(dehtmlspecialchars($$option2)) . "' AND
                        option3 = '" . addslashes(dehtmlspecialchars($$option3)) . "' AND
                        option4 = '" . addslashes(dehtmlspecialchars($$option4)) . "' AND
                        option5 = '" . addslashes(dehtmlspecialchars($$option5)) . "' AND
                        option6 = '" . addslashes(dehtmlspecialchars($$option6)) . "' AND
                        option7 = '" . addslashes(dehtmlspecialchars($$option7)) . "' AND
                        option8 = '" . addslashes(dehtmlspecialchars($$option8)) . "' AND
                        option9 = '" . addslashes(dehtmlspecialchars($$option9)) . "' AND
                        option10 = '" . addslashes(dehtmlspecialchars($$option10)) . "'";

        if (!$sql_result = mysql_query($query, $sock)) {
          echo mysql_error();
          exit();
        }


        // If this is a valid item, add it to the bundle
        if (mysql_num_rows($sql_result) == 1) {
          $sql_result_row = mysql_fetch_row($sql_result);
          $bundle->add_item(stripslashes($sql_result_row[0]), $$quantity);
          $valid_bundle = 1;
        }
      }
    }

    if (!$valid_bundle)
      continue;

    // See if a bundle name of type X-name, where X is a number,
    // exists. If it doesn't, use the name of the first item
    // in the bundle.
    $bundle_name = $h . "-name";

    if (!$$bundle_name) {
      $bundle_item = $bundle->items[0];
      $$bundle_name = $bundle_item->name;
    }

    $bundle->name = $$bundle_name;

    $quantity = $h . "-quantity";

    $order->add_bundle($bundle, $$quantity);
  }

  $order->update();

  header("Location: $redirect_url");
  exit();
?>
