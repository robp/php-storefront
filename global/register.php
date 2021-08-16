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

  // Make sure the user can't pass their own uid variable.
  if (isset($uid))
    unset($uid);

  // Grab the uid session variable, if it exists.
  session_register("uid");

  if (isset($uid)) {
    $user = new User(0, 0, $uid);
    if ($user->id) {
      $address = $user->getdefaultaddress();
      $update = 1;
    }
    else {
      echo "Invalid UID.";
      exit();
    }
  }

  if (!$referer)
    $referer = getenv("HTTP_REFERER");

  if (!$referer)
    $referer = $config->store_url;

  if ($step && (!strlen($username) || !strlen($password) || !strlen($passwordverify)))
    $errmsg .= "Please fill in all required fields.<br>";

  $username = strtolower(trim($username));
  if (strlen(ereg_replace("([[:alpha:]]|[[:digit:]])*", "", $username)))
    $errmsg .= "Invalid character(s) in username.<br>";

  if (db_item_exists("username", "'$username'", "users") && !$update)
    $errmsg .= "Username already exists. Please choose a new username.<br>";

  if ($password != $passwordverify)
    $errmsg .= "Passwords do not match.<br>";

  if (strlen($phone1) && (strlen($phone1) < 10))
    $errmsg .= "Please include area code in phone numbers.<br>";

  if (strlen($phone2) && (strlen($phone2) < 10))
    $errmsg .= "Please include area code in phone numbers.<br>";

  if (strlen($phone3) && (strlen($phone3) < 10))
    $errmsg .= "Please include area code in phone numbers.<br>";

  $required_array = explode(",", $required);

  if (strlen($username) && strlen($password) && strlen($passwordverify)) {
    for ($a = 0; $a < count($required_array); $a++) {
      $varname = $required_array[$a];

      if (!strlen($$varname)) {
        $errmsg .= "Please fill in all required fields.<br>";
        break;
      }
    }
  }

  if (!$age)
    $age = 0;

  if (!$update) {
    $user = new User();
    $address = new Address();
  }


  if ($step) {
    $user->gid = 1;
    $user->username = $username;
    $user->password = $password;
    $address->description = $user->username;
    $address->title = htmlspecialchars(trim($title));
    $address->fname = htmlspecialchars(trim($fname));
    $address->mname = htmlspecialchars(trim($mname));
    $address->lname = htmlspecialchars(trim($lname));
    $address->company = htmlspecialchars(trim($company));
    $address->address1 = htmlspecialchars(trim($address1));
    $address->address2 = htmlspecialchars(trim($address2));
    $address->address3 = htmlspecialchars(trim($address3));
    $address->city = htmlspecialchars(trim($city));
    $address->state = htmlspecialchars(trim($state));
    $address->country = htmlspecialchars(trim($country));
    $address->zipcode = htmlspecialchars(trim($zipcode));
    $address->phone1 = htmlspecialchars(trim($phone1));
    $address->phone2 = htmlspecialchars(trim($phone2));
    $address->phone3 = htmlspecialchars(trim($phone3));
    $address->email = htmlspecialchars(trim($email));
    $address->url = htmlspecialchars(trim($url));
    $address->age = $age;
    $address->gender = $gender[0];
    $address->marital = $marital[0];
    $address->notify = 1;
  }

  if (!$step || $errmsg) 
    $tmpl_url = $config->template_url . "register1.html";
  elseif (!$url)
    $tmpl_url = $config->template_url . "register2.html";

  if ($tmpl_url) {
    $file = fopen($tmpl_url, "r");

    if ($file) {
      // Read the whole template file into $result
      while (!feof($file)) {
        $result .= fgets($file, 1024);
      }

      fclose($file);

      $result = parse_dynamic_page($result, 0, 0, 0);

      $result = str_replace("%ERROR_MESSAGE%", $errmsg, $result);
      $result = str_replace("%HTTP_REFERER%", $referer, $result);
      $result = str_replace("%URL%", $url, $result);

      $result = str_replace("%USERNAME%", $user->username, $result);

      if ($update && !$step) {
        $result = str_replace("%PASSWORD%", $user->password, $result);
        $result = str_replace("%PASSWORDVERIFY%", $user->password, $result);
      }
      else {
        $result = str_replace("%PASSWORD%", $password, $result);
        $result = str_replace("%PASSWORDVERIFY%", $passwordverify, $result);
      }

      $result = str_replace("%TITLE%", $address->title, $result);
      $result = str_replace("%FNAME%", $address->fname, $result);
      $result = str_replace("%MNAME%", $address->mname, $result);
      $result = str_replace("%LNAME%", $address->lname, $result);
      $result = str_replace("%COMPANY%", $address->company, $result);
      $result = str_replace("%ADDRESS1%", $address->address1, $result);
      $result = str_replace("%ADDRESS2%", $address->address2, $result);
      $result = str_replace("%ADDRESS3%", $address->address3, $result);
      $result = str_replace("%CITY%", $address->city, $result);
      $result = str_replace("%STATE%", $address->state, $result);
      $result = str_replace("%COUNTRY%", $address->country, $result);
      $result = str_replace("%ZIPCODE%", $address->zipcode, $result);
      $result = str_replace("%PHONE1%", $address->phone1, $result);
      $result = str_replace("%PHONE2%", $address->phone2, $result);
      $result = str_replace("%PHONE3%", $address->phone3, $result);
      $result = str_replace("%EMAIL%", $address->email, $result);
      $result = str_replace("%URL%", $address->url, $result);
      $result = str_replace("%AGE%", $address->age, $result);
      $result = str_replace("%GENDER%", $address->gender, $result);
      $result = str_replace("%MARITAL%", $address->marital, $result);

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
    }
    else {
      echo "Error opening template file.";
      exit();
    }
  }
    
  if ($step == 1 && !$errmsg) {
    if ($update) {
      $user->update();
      $address->update();
    }
    else {
      $user->insert();
      $address->uid = $user->id;
      $address->insert();
    }
  }

  if ($url && $step == 1 && !$errmsg)
    header ("Location: " . $url . "\n\n");
  else
    echo $result;
?>
