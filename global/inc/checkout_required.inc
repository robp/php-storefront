<?php
  $required1 = array("bill_first_name", "bill_last_name", "bill_email",
                     "bill_address_1", "bill_city", "bill_state",
                     "bill_zipcode");
  $required2 = array("ship_first_name", "ship_last_name",
                     "ship_address_1", "ship_city", "ship_state",
                     "ship_zipcode");
  $required3 = array("cc_type", "cc_number",
                     "cc_exp_month", "cc_exp_year");

  // Check to see that all required form inputs are found
  if ($step) {
    for ($i = 0; $i < sizeof($required1); $i++) {
      if (!$$required1[$i]) {
        $missing_item = $required1[$i];
        $missing_info = 1;
        break;
      }
    }

    if (!($bill_day_phone_area_code && 
          $bill_day_phone_exchange &&
          $bill_day_phone_number))
      $missing_info = 1;

    if (!$ship_same_as_bill) {
      for ($i = 0; $i < sizeof($required2); $i++) {
        if (!$$required2[$i]) {
          $missing_item = $required2[$i];
          $missing_info = 1;
          break;
        }
      }
    }
    else {
      $ship_first_name = $bill_first_name;
      $ship_last_name = $bill_last_name;
      $ship_company_name = $bill_company_name;
      $ship_address_1 = $bill_address_1;
      $ship_address_2 = $bill_address_2;
      $ship_city = $bill_city;
      $ship_state = $bill_state;
      $ship_zipcode = $bill_zipcode;
      $ship_day_phone_area_code = $bill_day_phone_area_code;
      $ship_day_phone_exchange = $bill_day_phone_exchange;
      $ship_day_phone_number = $bill_day_phone_number;
    }
  }


  if ($step == 2) {
    for ($i = 0; $i < sizeof($required3); $i++) {
      if (!$$required3[$i]) {
        $missing_item = $required3[$i];
        $missing_info = 1;
        break;
      }
    }

    if (USE_SHIPPING && !$ship_method)
      $missing_info = 1;
  }
?>
