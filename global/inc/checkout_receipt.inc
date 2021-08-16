<?php
    $msg .= "
============================================================================

" . center($config->store_name, 76) . "
" . center($config->store_address_1, 76) . "
";

    if ($config->store_address_2)
      $msg .= center($config->store_address_2, 76) . "\n";

    $msg .= center($config->store_city . ", " . $config->store_state, 76) . "
" . center($config->store_country, 76) . "
" . center($config->store_zip, 76) . "

ORDER INFORMATION

";

    $line = sprintf("%-55s %20s\n", ("Order #: " . $order_id),
                    date("M d Y H:i:s", $transact->timestamp));
    $msg .= $line;

    $msg .= "Auth. #: " . $transact->authno . "

Item                            Part No.        Unit Price  Qty.       Price
----------------------------------------------------------------------------
";

    // For each bundle in the order...
    for ($i = 0; $i < count($order->bundles); $i++) {
      $bundle = $order->bundles[$i];
      $bundle_price = 0;

      // For each item in the bundle...
      for ($j = 0; $j < count($bundle->items); $j++) {
        $bundle_item = $bundle->items[$j];

        // Calculate the item's cost and weight
        $bundle_item_cost = $bundle_item->price * $bundle_item->quantity;

        // Add the item's cost to the bundle price
        $bundle_price += $bundle_item_cost;
      }

      $line = sprintf("%-46s %11s %4s %12s\n", str_replace("\r", "", str_replace("\n", "", substr($bundle->name, 0, 46))),
         "\$" . format_money($bundle_price), $bundle->quantity,
         "\$" . format_money($bundle_price * $bundle->quantity));

      $msg .= $line;

      // For each item in the bundle, print it out
      for ($j = 0; $j < count($bundle->items); $j++) {
        $bundle_item = $bundle->items[$j];

        $line = sprintf("  %-28s  %-15s\n",
                substr($bundle_item->name, 0, 28), $bundle_item->sku);

        $msg .= $line;
      }

      $msg .= "----------------------------------------------------------------------------\n";
    }

    $line = sprintf("%76s\n", "PST \$" . format_money($pst_total));
    $msg .= $line;
    $line = sprintf("%76s\n", "GST \$" . format_money($gst_total));
    $msg .= $line;
    $line = sprintf("%76s\n", "Total \$" . format_money($total));
    $msg .= $line;

    if (USE_SHIPPING) {
      $msg .= "\nShipping method: " . $ship_desc;
      $msg .= "\nShipping cost: \$";
      if ($ship_cost < 0)
        $msg .= "N/A";
      else
        $msg .= format_money($ship_cost) . " (added to your Order Total, above)";
      $msg .= "\n\n";

    }
    else {
      $msg .= "\nThe cost of shipping will be added to your total, above.\n\n";
    }

    // Add the Credit Card number to the pgp-potential message
    // The plain message gets only the last 4 digits of the number
    $line = "CREDIT CARD: ";
    $msg .= $line;
    $msg_cc = $msg;

    // Create the "cleartext" cc number
    $cc_plain = safe_CC($cc_number);

    $msg    .= $cc_plain . "\n";
    $msg_cc .= $cc_number . "\n";

    $line = "             " . $cc_type . "  Exp. " . $cc_exp_month . "/" . $cc_exp_year . "\n\n";
    $msg    .= $line;
    $msg_cc .= $line;

    $line = "BILLING INFORMATION                     SHIPPING INFORMATION\n\n";
    $msg    .= $line;
    $msg_cc .= $line;

    $line = sprintf("%-39s %-39s\n",
       ($bill_first_name . " " . $bill_last_name),
       ($ship_first_name . " " . $ship_last_name));
    $msg    .= $line;
    $msg_cc .= $line;

    $line = sprintf("%-39s %-39s\n",
       ($bill_company_name),
       ($ship_company_name));
    $msg    .= $line;
    $msg_cc .= $line;

    $line = sprintf("%-39s %-39s\n",
       ($bill_address_1 . ($bill_address_2 ? ", " . $bill_address_2 : "")),
       ($ship_address_1 . ($ship_address_2 ? ", " . $ship_address_2 : "")));
    $msg    .= $line;
    $msg_cc .= $line;

    $line = sprintf("%-39s %-39s\n",
       ($bill_city . ", " . $bill_state),
       ($ship_city . ", " . $ship_state));
    $msg    .= $line;
    $msg_cc .= $line;

    $line = sprintf("%-39s %-39s\n", $bill_zipcode, $ship_zipcode);
    $msg    .= $line;
    $msg_cc .= $line;

    $line = sprintf("%-39s %-39s\n",
        ("Day: (" . $bill_day_phone_area_code . ") " . $bill_day_phone_exchange . "-" . $bill_day_phone_number),
        ("Day: (" . $ship_day_phone_area_code . ") " . $ship_day_phone_exchange . "-" . $ship_day_phone_number));
    $msg    .= $line;
    $msg_cc .= $line;

    $line = sprintf("%-39s\n",
        ("Eve: (" . $bill_eve_phone_area_code . ") " . $bill_eve_phone_exchange . "-" . $bill_eve_phone_number));
    $msg    .= $line;
    $msg_cc .= $line;

    $line = sprintf("%-39s\n",
        ("Fax: (" . $bill_fax_area_code . ") " . $bill_fax_exchange . "-" . $bill_fax_number));
    $msg    .= $line;
    $msg_cc .= $line;

    $line = sprintf("%-39s\n",
        ("Email: " . $bill_email));
    $msg    .= $line;
    $msg_cc .= $line;


    $line = "\n============================================================================

";
    $msg    .= $line;
    $msg_cc .= $line;
?>
