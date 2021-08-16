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
    $referer = $config->store_url;

  if (!$basket_id) {
    header("Location: " . $config->viewbasket_url . "?referer=$referer");
    exit();
  }

  $logged_in = 0;

  // Make sure the user can't pass their own uid variable.
  if (isset($uid))
    unset($uid);

  // Grab the uid session variable, if it exists.
  session_register("uid");

  if (isset($uid)) {
    $user = new User(0, 0, $uid);

    if (!$user->id) {
      echo "Invalid UID.";
      exit();
    }

    $bill = $user->getdefaultaddress();

    if (!$bill->id) {
      echo "No default address.";
      exit();
    }

    $logged_in = 1;

    // If they're not a temp user, get all their addressbook entries
    if ($user->gid != 0)
      $addresses = $user->getaddresses();
  }

  session_register("ship_to");

  if (isset($ship_to)) {
    $ship = new Address (0, $ship_to);

    if ($ship->uid != $user->id) {
      echo "Invalid ship_to ID.";
      exit();
    }
  }

  // Get and process the "required" steps to ensure all required form
  // input is present.  If not, the missing_info variable will be set to 1.
  require("inc/checkout_required.inc");

  // Perform step-specific actions for the checkout process
  if ($step == 1) {
    if (!$logged_in) {
      $user = new User();
      $bill = new Address();
      $ship = new Address();
    }

    $user->gid = 0;
    $user->username = "*";		// Use * for temp accounts
    $user->password = "*";		// Use * for temp accounts
    
    $bill->title = $bill_title;
    $bill->fname = $bill_fname;
    $bill->mname = $bill_mname;
    $bill->lname = $bill_lname;
    $bill->company = $bill_company;
    $bill->address1 = $bill_address1;
    $bill->address2 = $bill_address2;
    $bill->address3 = $bill_address3;
    $bill->city = $bill_city;
    $bill->state = $bill_state;
    $bill->country = $bill_country;
    $bill->zipcode = $bill_zipcode;
    $bill->phone1 = $bill_phone1;
    $bill->phone2 = $bill_phone2;
    $bill->phone3 = $bill_phone3;
    $bill->email = $bill_email;
    $bill->url = $bill_url;
    $bill->age = $bill_age;
    if (!$bill->age)
      $bill->age = 0;
    $bill->gender = $bill_gender;
    $bill->marital = $bill_marital;
    $bill->notify = $bill_notify;
    if (!$bill->notify)
      $bill->notify = 1;

    $ship->title = $ship_title;
    $ship->fname = $ship_fname;
    $ship->mname = $ship_mname;
    $ship->lname = $ship_lname;
    $ship->company = $ship_company;
    $ship->address1 = $ship_address1;
    $ship->address2 = $ship_address2;
    $ship->address3 = $ship_address3;
    $ship->city = $ship_city;
    $ship->state = $ship_state;
    $ship->country = $ship_country;
    $ship->zipcode = $ship_zipcode;
    $ship->phone1 = $ship_phone1;
    $ship->phone2 = $ship_phone2;
    $ship->phone3 = $ship_phone3;
    $ship->email = $ship_email;
    $ship->url = $ship_url;
    $ship->age = $ship_age;
    if (!$ship->age)
      $ship->age = 0;
    $ship->gender = $ship_gender;
    $ship->marital = $ship_marital;
    $ship->notify = $ship_notify;
    if (!$ship->notify)
      $ship->notify = 0;

    if (!$missing_info) {
      if ($logged_in) {
        $user->update();
        $bill->update();
        $ship->update();
      }
      else {
        $user->insert();			// Insert a temp record
        $bill->uid = $user->id;
        $ship->uid = $user->id;
        $bill->insert();
        $ship->insert();

        $uid = $user->id;
        $ship_to = $ship->id;

        session_register("uid");		//   temporary session
        session_register("ship_to");		//   temporary session
      }
    }
  }
  elseif ((!$step && $logged_in && $user->gid != 0) || ($step == 2 && $missing_info)) {
    // If this is a real user, and they're logged in, and they're viewing
    // the "choose address" page, remove the current ship_to b/c PHP won't
    // let me reset the variable to another value without unregistering
    // it first
    session_unregister("ship_to");
  }

  $charge_pst = 0;
  $charge_gst = 0;

  for ($i = 0; $i < count($PROVINCES_STATES); $i++) {
    if ($ship->state == $PROVINCES_STATES[$i][0]) {
      $charge_gst = 1;
      $break;
    }
  }

  if ($ship->state == $config->shipping_origin)
    $charge_pst = 1;

  // Get the order and the discount value
  $order = new Order();
  $order->get_by_basket_id($basket_id);

  if (!$charge_pst) $order->pst = 0;
  if (!$charge_gst) $order->gst = 0;

  // Put together the shipping types/costs arrays
  if ($ship->id) {
    // Determine which shipping zone the customer is in
    $shipzoneassign = new ShippingZoneAssignment(0, $ship->state);

    if ($shipzoneassign->zone_id) {
      $shipzone = new ShippingZone(0, $shipzoneassign->zone_id);

      // Get the shipping types available for that zone
      $shiptypes = get_shiptypes_by_zone($shipzone->id);

      // Calculate the costs for each of the shipping types for that zone
      for ($i = 0; $i < count($shiptypes); $i++) {
        if ($config->shipping_model == 0) {
          $cost = get_shipcost_price($shiptypes[$i]->id, 
                                     $shipzone->id, $order->cost);
          if ($cost) {
            $shiptypes_display[] = $shiptypes[$i];
            $shipcosts_display[] = format_money($cost);
            $shipcosts[] = $cost;
          }
        }
        elseif ($config->shipping_model == 1) {
          $cost = get_shipcost_weight($shiptypes[$i]->id,
                                      $shipzone->id, $order->weight);
          if ($cost) {
            $shiptypes_display[] = $shiptypes[$i];
            $shipcosts_display[] = format_money($cost);
            $shipcosts[] = $cost;
          }
        }
        elseif ($config->shipping_model == 2) {
          $cost = get_shipcost_price($shiptypes[$i]->id, 
                                     $shipzone->id, $order->cost);
          if ($cost) {
            $shiptypes_display[] = $shiptypes[$i];
            $shipcosts_display[] = format_money($cost);
            $shipcosts[] = $cost;
          }
        }
      }
    }

    if (!count($shiptypes_display)) {
      $shiptype = new ShippingType();
      $shiptype->id = -1;
      $shiptype->description = "Shipping information not available";

      $shiptypes_display[] = $shiptype;
      $shipcosts_display[] = "N/A";
      $shipcosts[] = "-1";
    }

    // Calculate user selected shipping type and cost
    for ($a = 0; $a < count($shiptypes_display); $a++) {
      if ($shiptypes_display[$a]->id == $ship_method) {
        $ship_desc = $shiptypes_display[$a]->description;
        $ship_cost = $shipcosts[$a];
      }
    }
  }

  // Perform final step-specific tasks for checkout
  // This is all for the last step of the process, where
  // the CC gets charged, the emails get sent out, and the
  // receipt is placed in the db.

  if ($step == 3 && !$missing_info) {
    // Check the integrity of the form data
    if ($cc_exp_year < date("Y")) {
      $errmsg = "Credit card is expired.";
      $missing_info = 1;
    }
    if ($cc_exp_year == date("Y")) {
      if ($cc_exp_month < date("m")) {
        $errmsg = "Credit card is expired.";
        $missing_info = 1;
      }
    }
    if (!validateCC($cc_number, $cc_type)) {
      $errmsg = "Invalid credit card number.";
      $missing_info = 1;
    }
  }

  if ($step == 3 && !$missing_info) {
    // Put together the grand total, including
    // all taxes and shipping charges
    $total = $order->cost + $order->pst + $order->gst +
             (USE_SHIPPING == 1 ? $ship_cost : 0);

    // Create an order id
    $order_id = sprintf("%07d", $order->id);


    // Take the spaces out of the CC number
    $cc_number = ereg_replace('[-[:space:]]', '', $cc_number);

    // Create the 4 digit expiry date string
    $cc_exp_date = $cc_exp_month . $cc_exp_year[2] . $cc_exp_year[3];

    // Take the decimal out of the total price (shipping included)
    $cc_cost = ereg_replace('[^[:digit:]]', '', format_money($total));

    // Temporary $1.00 cost for testing
    //$cc_cost = 100;

    $exec_str = CC_PROG_NAME . " " . CC_SERVER . " " . CC_DEVICE .
            " " . $config->cc_terminal_id . " " . CC_OPERID . " " . $cc_number .
            " " . $cc_exp_date . " " . $cc_cost . " " . $order_id .
            " " . CC_ACTION;

    // Execute the transaction
    if ($config->cc_live) {
      // Changed the old method b/c it stopped working somewhere in
      // PHP version 4
      //$cc_result = exec (EscapeShellCmd($exec_str));
      $cmd = EscapeShellCmd($exec_str);
      $cc_result = `$cmd`;

      // Read the result into a new Transaction and add it to the database
      $result_array = explode(",", $cc_result);

      $transact = new Transaction();
      $transact->timestamp = time();
      $transact->refno = trim($result_array[0]);
      $transact->authno = trim($result_array[1]);
      $transact->pridisplay = trim($result_array[2]);
      $transact->result = trim($result_array[3]);
      $transact->insert();

      // If we receive didn't receive an authno, or if the result isn't "0000",
      // then the transaction failed. It would be nice to have a list of
      // result codes so that the response could be more accurate.
      if ($transact->result == "8001") {
        $missing_info = 1;
        $errmsg = "Credit card is expired. Please another card.";
      }
      elseif ($transact->result == "9998") {
        $missing_info = 1;
        $errmsg = "The transaction timed out. Please try again.";
      }
      elseif ($transact->result == "9999") {
        $missing_info = 1;
        $errmsg = "A transmission error occurred. Please try again later.";
      }
      elseif (!$transact->authno || $transact->result != "0000") {
        $missing_info = 1;
        $errmsg = "Your credit card was declined. Please try another card.";
      }
    }
  }


  if ($step == 3 && !$missing_info) {
    // Create the email messages
    $from = "From: " . $config->email_sender_name . " <" . $config->email_sender_address . ">";
    $subject = $config->store_name . " - Order #" . $order_id;

    // Generate the receipt.  This will create the "msg" variable
    // which contains the text receipt, and the "msg_cc" variable
    // which contains the same receipt, with the actual cc number
    // in it
    require("inc/checkout_receipt.inc");

    // Create and insert a receipt object if this was
    // a live transaction
    if ($config->cc_live) {
      $receipt = new Receipt();
      $receipt->id = $transact->refno;
      $receipt->uid = $user->id;
      $receipt->receipt = $msg;
      $receipt->insert();
    }

    // msg goes to the merchant
    // msg2 goes to the customer
    $msg2 = $config->email_cust_header . $msg . $config->email_cust_footer;

    // If we are using PGP, encrypt the $msg to the merchant
    // and include the credit card number
    if ($config->pgp_use) {
      $command  = "LOGNAME=httpd; HOME=/usr/local/apache; ";
      $command .= "USER=httpd; USERNAME=httpd; ";
      $command .= "echo \"" .  str_replace("\$", "\\$", 
                                           addslashes($msg_cc)) . "\" | ";
      $command .= PGP_BIN . " -feat \"" . PGP_RCPT_PUBKEY_ID . "\"";

      exec ($command, $execresult);

      for ($a = 0; $a < count($execresult); $a++) {
        $pgpmsg .= $execresult[$a] . "\n";
        // Don't know why this was ever in here :)
        //$pgpmsg = stripslashes($pgpmsg);
      }
    }

    if ($config->pgp_use)
      mail($config->billing_email, $subject, $pgpmsg, $from);
    else
      mail($config->billing_email, $subject, $msg, $from);

    mail($bill->email, $subject, $msg2, $from);

    $order->delete();
    SetCookie("basket_id", "", 0, "", "." . $config->domain_name);

    // If this was a temp user, delete 'em
    if ($user->gid == 0)
      $user->delete();

    session_destroy();
  }

  // Determine which template to use and display
  if ((!$step && !$logged_in) || (!$step && $logged_in && $user->gid == 0) ||
      ($step == 1 && $missing_info))
    $tmpl_url = $config->template_url . "checkout1.html";
  elseif ((!$step && $logged_in && $user->gid != 0) || ($step == 2 && $missing_info))
    $tmpl_url = $config->template_url . "checkout2.html";
  elseif ((($step == 1 || $step == 2) && !$missing_info) ||
          ($step == 3 && $missing_info))
    $tmpl_url = $config->template_url . "checkout3.html";
  elseif ($step == 3 && !$missing_info)
    $tmpl_url = $config->template_url . "checkout4.html";
  else {
    echo "Unknown error opening template file.<p>";
    exit();
  }

  // Open the file
  $file = fopen($tmpl_url, "r");

  if ($file) {
    // Read the whole template file into $result
    while (!feof($file)) {
      $result .= fgets($file, 1024);
    }

    fclose($file);
  }
  else {
    echo "Error opening template.";
    exit();
  }

  // Replace some things that can appear anywhere in the page, but that
  // wouldn't appear in a category or product page since we save those
  // for the bundle parsing

  if ($errmsg) $msg = $errmsg;

  $result = parse_dynamic_page($result, 0, 0, 0);

  $result = str_replace("%HTTP_REFERER%", $referer, $result);
  $result = str_replace("%MSG%", $msg, $result);

  $result = str_replace("%BILL_AGE%", $bill->age, $result);
  $result = str_replace("%BILL_GENDER%", $bill->gender, $result);
  $result = str_replace("%BILL_MARITAL%", $bill->marital, $result);
  $result = str_replace("%BILL_TITLE%", $bill->title, $result);
  $result = str_replace("%BILL_FNAME%", $bill->fname, $result);
  $result = str_replace("%BILL_MNAME%", $bill->mname, $result);
  $result = str_replace("%BILL_LNAME%", $bill->lname, $result);
  $result = str_replace("%BILL_COMPANY%", $bill->company, $result);
  $result = str_replace("%BILL_ADDRESS1%", $bill->address1, $result);
  $result = str_replace("%BILL_ADDRESS2%", $bill->address2, $result);
  $result = str_replace("%BILL_ADDRESS3%", $bill->address3, $result);
  $result = str_replace("%BILL_CITY%", $bill->city, $result);
  $result = str_replace("%BILL_STATE%", $bill->state, $result);
  $result = str_replace("%BILL_COUNTRY%", $bill->country, $result);
  $result = str_replace("%BILL_ZIPCODE%", $bill->zipcode, $result);
  $result = str_replace("%BILL_PHONE1%", $bill->phone1, $result);
  $result = str_replace("%BILL_PHONE2%", $bill->phone2, $result);
  $result = str_replace("%BILL_PHONE3%", $bill->phone3, $result);
  $result = str_replace("%BILL_EMAIL%", $bill->email, $result);
  $result = str_replace("%BILL_URL%", $bill->url, $result);

  $result = str_replace("%SHIP_AGE%", $ship->age, $result);
  $result = str_replace("%SHIP_GENDER%", $ship->gender, $result);
  $result = str_replace("%SHIP_MARITAL%", $ship->marital, $result);
  $result = str_replace("%SHIP_TITLE%", $ship->title, $result);
  $result = str_replace("%SHIP_FNAME%", $ship->fname, $result);
  $result = str_replace("%SHIP_MNAME%", $ship->mname, $result);
  $result = str_replace("%SHIP_LNAME%", $ship->lname, $result);
  $result = str_replace("%SHIP_COMPANY%", $ship->company, $result);
  $result = str_replace("%SHIP_ADDRESS1%", $ship->address1, $result);
  $result = str_replace("%SHIP_ADDRESS2%", $ship->address2, $result);
  $result = str_replace("%SHIP_ADDRESS3%", $ship->address3, $result);
  $result = str_replace("%SHIP_CITY%", $ship->city, $result);
  $result = str_replace("%SHIP_STATE%", $ship->state, $result);
  $result = str_replace("%SHIP_COUNTRY%", $ship->country, $result);
  $result = str_replace("%SHIP_ZIPCODE%", $ship->zipcode, $result);
  $result = str_replace("%SHIP_PHONE1%", $ship->phone1, $result);
  $result = str_replace("%SHIP_PHONE2%", $ship->phone2, $result);
  $result = str_replace("%SHIP_PHONE3%", $ship->phone3, $result);
  $result = str_replace("%SHIP_EMAIL%", $ship->email, $result);
  $result = str_replace("%SHIP_URL%", $ship->url, $result);

  while (ereg("%LIST_PROVINCES_SELECT\(([[:alpha:][:digit:][:space:]-]*)\)%", $result, $regs)) {
    $str2 = "%LIST_PROVINCES_SELECT(" . $regs[1] . ")%";
    $result = str_replace($str2, list_provinces_select($regs[1]), $result);
  }

  while (ereg("%LIST_STATES_SELECT\(([[:alpha:][:digit:][:space:]-]*)\)%", $result, $regs)) {
    $str2 = "%LIST_STATES_SELECT(" . $regs[1] . ")%";
    $result = str_replace($str2, list_states_select($regs[1]), $result);
  }

  while (ereg("%LIST_PROVINCES_CODE_SELECT\(([[:alpha:][:digit:][:space:]-]*)\)%", $result, $regs)) {
    $str2 = "%LIST_PROVINCES_CODE_SELECT(" . $regs[1] . ")%";
    $result = str_replace($str2, list_provinces_code_select($regs[1]), $result);
  }

  while (ereg("%LIST_STATES_CODE_SELECT\(([[:alpha:][:digit:][:space:]-]*)\)%", $result, $regs)) {
    $str2 = "%LIST_STATES_CODE_SELECT(" . $regs[1] . ")%";
    $result = str_replace($str2, list_states_code_select($regs[1]), $result);
  }

  while (ereg("%LIST_COUNTRIES_SELECT\(([[:alpha:][:digit:][:space:]-]*)\)%", $result, $regs)) {
    $str2 = "%LIST_COUNTRIES_SELECT(" . $regs[1] . ")%";
    $result = str_replace($str2, list_countries_select($regs[1]), $result);
  }


  // Display the shopping basket information, if required

  // Replace some things that can appear anywhere in the page, but that
  // wouldn't appear in a category or product page since we save those
  // for the bundle parsing
  $result = str_replace("%ORDER_SUBTOTAL%",
                         format_money($order->cost), $result);
  $result = str_replace("%ORDER_TOTAL%",
                         format_money($order->cost + $order->pst + $order->gst),
                         $result);
  $result = str_replace("%ORDER_GRANDTOTAL%", format_money($total), $result);
  $result = str_replace("%ORDER_WEIGHT%", $order->weight, $result);
  $result = str_replace("%ORDER_EXPIRY_TIME%", $order->expiry_time, $result);
  $result = str_replace("%HTTP_REFERER%", $referer, $result);
  $result = str_replace("%DISCOUNT_VALUE%",
                         format_money($order->discount), $result);
  $result = str_replace("%ORDER_PST%", format_money($order->pst), $result);
  $result = str_replace("%ORDER_GST%", format_money($order->gst), $result);
  $result = str_replace("%ORDER_REFNO%", $transact->refno, $result);
  $result = str_replace("%ORDER_AUTHNO%", $transact->authno, $result);
  $result = str_replace("%ORDER_RECEIPT%", $receipt->receipt, $result);

  $result_array = split ("(%BEGIN_DISCOUNT%|%END_DISCOUNT%)", $result);

  if (count($result_array) == 3) {
    $result = $result_array[0];
    if ($order->discount > 0) {
      $result .= $result_array[1];
    }
    $result .= $result_array[2];
  }

  // Display the shopping basket, if required.

  $result_array = split ("(%BEGIN_LOOP%|%END_LOOP%)", $result);

  if (count($result_array) == 3) {
    $result = $result_array[0];
    for ($i = 0; $i < count($order->bundles); $i++) {
      $result .= parse_basket_bundle($order->bundles[$i], $result_array[1]);
    }
    $result .= $result_array[2];
  }


  // Display the addressbook, if required

  $result_array = split ("(%BEGIN_ADDRESSBOOK_LOOP%|%END_ADDRESSBOOK_LOOP%)", $result);

  if (count($result_array) == 3) {
    $result = $result_array[0];

    for ($x = 0; $x < count($addresses); $x++) {
      $result_loop = $result_array[1];

      $result_loop = str_replace("%ADDRESS_ID%", $addresses[$x]->id, $result_loop);
      $result_loop = str_replace("%ADDRESS_DESCRIPTION%", $addresses[$x]->description, $result_loop);
      $result_loop = str_replace("%ADDRESS_TITLE%", $addresses[$x]->title, $result_loop);
      $result_loop = str_replace("%ADDRESS_FNAME%", $addresses[$x]->fname, $result_loop);
      $result_loop = str_replace("%ADDRESS_MNAME%", $addresses[$x]->mname, $result_loop);
      $result_loop = str_replace("%ADDRESS_LNAME%", $addresses[$x]->lname, $result_loop);
      $result_loop = str_replace("%ADDRESS_COMPANY%", $addresses[$x]->company, $result_loop);
      $result_loop = str_replace("%ADDRESS_ADDRESS1%", $addresses[$x]->address1, $result_loop);
      $result_loop = str_replace("%ADDRESS_ADDRESS2%", $addresses[$x]->address2, $result_loop);
      $result_loop = str_replace("%ADDRESS_ADDRESS3%", $addresses[$x]->address3, $result_loop);
      $result_loop = str_replace("%ADDRESS_CITY%", $addresses[$x]->city, $result_loop);
      $result_loop = str_replace("%ADDRESS_STATE%", $addresses[$x]->state, $result_loop);
      $result_loop = str_replace("%ADDRESS_COUNTRY%", $addresses[$x]->country, $result_loop);
      $result_loop = str_replace("%ADDRESS_ZIPCODE%", $addresses[$x]->zipcode, $result_loop);
      $result_loop = str_replace("%ADDRESS_PHONE1%", $addresses[$x]->phone1, $result_loop);
      $result_loop = str_replace("%ADDRESS_PHONE2%", $addresses[$x]->phone2, $result_loop);
      $result_loop = str_replace("%ADDRESS_PHONE3%", $addresses[$x]->phone3, $result_loop);
      $result_loop = str_replace("%ADDRESS_EMAIL%", $addresses[$x]->email, $result_loop);
      $result_loop = str_replace("%ADDRESS_URL%", $addresses[$x]->url, $result_loop);
      $result_loop = str_replace("%ADDRESS_NOTIFY%", $addresses[$x]->notify, $result_loop);
      $result_loop = str_replace("%ADDRESS_AGE%", $addresses[$x]->age, $result_loop);
      $result_loop = str_replace("%ADDRESS_GENDER%", $addresses[$x]->gender, $result_loop);
      $result_loop = str_replace("%ADDRESS_MARITAL%", $addresses[$x]->marital, $result_loop);

      $result .= $result_loop;
    }

    $result .= $result_array[2];
  }

  // Display the shipping info, if required


  $result_array = split ("(%BEGIN_SHIPPING%|%END_SHIPPING%)", $result);

  if (count($result_array) == 3) {
    $result = $result_array[0];

    $result_array2 = split ("(%BEGIN_SHIPPING_LOOP%|%END_SHIPPING_LOOP%)", $result_array[1]);

    if (count($result_array2) == 3) {
      $result .= $result_array2[0];

      for ($x = 0; $x < count($shiptypes_display); $x++) {
        $result_loop = $result_array2[1];

        $result_loop = str_replace("%SHIPMETHOD_ID%", $shiptypes_display[$x]->id, $result_loop);
        $result_loop = str_replace("%SHIPMETHOD_NAME%", $shiptypes_display[$x]->description, $result_loop);
        $result_loop = str_replace("%SHIPMETHOD_COST%", $shipcosts_display[$x], $result_loop);

        $result .= $result_loop;
      }

      $result .= $result_array2[2];
    }

    $result .= $result_array[2];
  }

  echo $result;
?>
