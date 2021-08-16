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

  if (!$referer)
    $referer = getenv("HTTP_REFERER");

  if (!$referer)
    $referer = $config->store_url;

  // Make sure the user can't pass their own uid variable.
  if (isset($uid))
    unset($uid);

  // Grab the uid session variable, if it exists.
  session_register("uid");

  if (isset($uid)) {
    $user = new User(0, 0, $uid);
    if ($user->id) {
      $user_address = $user->getdefaultaddress();
      $addresses = $user->getaddresses();
    }
    else {
      echo "Invalid UID.";
      exit();
    }
  }
  else {
    echo "Access denied.";
    exit();
  }

  $tmpl_url = $config->template_url . "addressbook.html";

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

  $result = parse_dynamic_page($result, 0, 0, 0);
  $result = str_replace("%HTTP_REFERER%", $referer, $result);
  $result = str_replace("%MSG%", $msg, $result);
  $result = str_replace("%LIST_ADDRESSBOOK_OPTIONS%", $addressbook_options, $result);
  $result = str_replace("%USER_ID%", $user_address->id, $result);
  $result = str_replace("%USER_USERNAME%", $user_address->description, $result);
  $result = str_replace("%USER_DESCRIPTION%", $user_address->description, $result);
  $result = str_replace("%USER_TITLE%", $user_address->title, $result);
  $result = str_replace("%USER_FNAME%", $user_address->fname, $result);
  $result = str_replace("%USER_MNAME%", $user_address->mname, $result);
  $result = str_replace("%USER_LNAME%", $user_address->lname, $result);
  $result = str_replace("%USER_COMPANY%", $user_address->company, $result);
  $result = str_replace("%USER_ADDRESS1%", $user_address->address1, $result);
  $result = str_replace("%USER_ADDRESS2%", $user_address->address2, $result);
  $result = str_replace("%USER_ADDRESS3%", $user_address->address3, $result);
  $result = str_replace("%USER_CITY%", $user_address->city, $result);
  $result = str_replace("%USER_STATE%", $user_address->state, $result);
  $result = str_replace("%USER_COUNTRY%", $user_address->country, $result);
  $result = str_replace("%USER_ZIPCODE%", $user_address->zipcode, $result);
  $result = str_replace("%USER_PHONE1%", $user_address->phone1, $result);
  $result = str_replace("%USER_PHONE2%", $user_address->phone2, $result);
  $result = str_replace("%USER_PHONE3%", $user_address->phone3, $result);
  $result = str_replace("%USER_EMAIL%", $user_address->email, $result);
  $result = str_replace("%USER_URL%", $user_address->url, $result);
  $result = str_replace("%USER_NOTIFY%", $user_address->notify, $result);
  $result = str_replace("%USER_AGE%", $user_address->age, $result);
  $result = str_replace("%USER_GENDER%", $user_address->gender, $result);
  $result = str_replace("%USER_MARITAL%", $user_address->marital, $result);

  $result_array = split ("(%BEGIN_LOOP%|%END_LOOP%)", $result);

  if (count($result_array) == 3) {
    $result = $result_array[0];

    for ($x = 1; $x < count($addresses); $x++) {
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

  if ($errmsg)
    echo $errmsg;
  else
    echo $result;
?>
