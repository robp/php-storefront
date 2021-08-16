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

  // The url variable can contain a URL that displays a confirmation
  // page when the user logs in successfully.  If this isn't set, then
  // we just go back to the page they came from (source).
  if (!strlen($url))
    $url = "addressbook.phtml";

  // Make sure the user can't pass their own uid variable.
  if (isset($uid))
    unset($uid);

  // Grab the uid session variable, if it exists.
  session_register("uid");


  if (isset($uid)) {
    $user = new User(0, 0, $uid);
    if ($user->id) {
      if ($id) {
        $address = new Address(0, $id);
        if ($address->id && ($address->uid == $user->id)) {
          if ($delete) {
            $query = "DELETE FROM addressbook
                      WHERE id = " . $address->id;

            if (!$sql_result = mysql_query($query, $sock)) {
              echo mysql_error();
              html_exit();
            }

            header ("Location: " . $url . "?referer=" . $referer . "&msg=" . $errmsg . "\n\n");
            exit();
          }
          
          $update = 1;
        }
        else {
          echo "Invalid address ID.";
          exit();
        }
      }
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

  if (strlen($description) && !$update) {
    $query = "SELECT * FROM addressbook
              WHERE uid = " . $user->id . "
              AND description = '" . $description . "'";

    if (!$sql_result = mysql_query($query, $sock)) {
      echo mysql_error();
      html_exit();
    }

    if (mysql_num_rows($sql_result)) {
      $errmsg .= "Nickname already exists. Please choose a new Nickname.<br>";
    }
  }

  if (strlen($phone1) && (strlen($phone1) < 10))
    $errmsg .= "Please include area code in phone numbers.<br>";

  if (strlen($phone2) && (strlen($phone2) < 10))
    $errmsg .= "Please include area code in phone numbers.<br>";

  if (strlen($phone3) && (strlen($phone3) < 10))
    $errmsg .= "Please include area code in phone numbers.<br>";

  if ($step) {
    if (!strlen($description)) {
      $errmsg .= "Please fill in all required fields.<br>";
    }

    $required_array = explode(",", $required);

    for ($a = 0; $a < count($required_array); $a++) {
      $varname = $required_array[$a];

      if (!strlen($$varname) && strlen($description)) {
        $errmsg .= "Please fill in all required fields.<br>";
        break;
      }
    }
  }

  if (!$age)
    $age = 0;

  if (!$update)
    $address = new Address();

  if ($step) {
    $address->description = htmlspecialchars(trim($description));
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

  $tmpl_url = $config->template_url . "address.html";

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

    $result = str_replace("%ID%", $address->id, $result);
    $result = str_replace("%DESCRIPTION%", $address->description, $result);
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
    
  if ($step == 1 && !$errmsg) {
    if ($update) {
      $address->update();
    }
    else {
      $address->uid = $user->id;
      $address->insert();
    }
    $success = 1;
  }

  if (!$success)
    echo $result;
  else {
    $errmsg = urlencode("Addressbook updated.");
    header ("Location: " . $url . "?referer=" . $referer . "&msg=" . $errmsg . "\n\n");
    exit();
  }
?>
