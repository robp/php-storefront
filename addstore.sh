#!/bin/sh
# Script to create a new online store
# by Rob Pinciuc <robp@wincom.net>, 22 Sep 1999
#
# Modified continually over the next 5 months


# Define some variables
STOREFRONT=/usr/local/storefront
MYSQL=/usr/local/mysql

echo
echo "Store ID (lowercase letters only): "
read STOREID
if [ -z "$STOREID" ]; then
  echo "Come on, man, you can't leave the store id field empty..."
  exit
fi
echo

if [ -d "$STOREFRONT/$STOREID" ]; then
  echo "Specified store id already exists."
  exit
fi

echo
echo "Directory path for dyn link to global structure: "
read DYNDIR
if [ -z "$DYNDIR" ]; then
  echo "Come on, man, you can't leave the directory field empty..."
  exit
fi
echo

echo "Creating directory structure..."
mkdir $STOREFRONT/$STOREID
mkdir $STOREFRONT/$STOREID/admin
mkdir $STOREFRONT/$STOREID/inc
mkdir $STOREFRONT/$STOREID/cache

echo "Creating shopping scripts..."
cd $STOREFRONT/$STOREID
ln -s ../global/address.phtml
ln -s ../global/addressbook.phtml
ln -s ../global/addtobasket.phtml
ln -s ../global/category.phtml
ln -s ../global/checkout.phtml
ln -s ../global/img
ln -s ../global/index.phtml
ln -s ../global/login.phtml
ln -s ../global/logout.phtml
ln -s ../global/product.phtml
ln -s ../global/register.phtml
ln -s ../global/search.phtml
ln -s ../global/updatebasket.phtml
ln -s ../global/viewbasket.phtml

echo "Creating admin scripts..."
cd $STOREFRONT/$STOREID/admin
ln -s ../../global/admin/addcat.phtml
ln -s ../../global/admin/adddiscount.phtml
ln -s ../../global/admin/additem.phtml
ln -s ../../global/admin/addoption.phtml
ln -s ../../global/admin/addtemplate.phtml
ln -s ../../global/admin/clean_db.php
ln -s ../../global/admin/clearcache.phtml
ln -s ../../global/admin/config1.phtml
ln -s ../../global/admin/config2.phtml
ln -s ../../global/admin/config3.phtml
ln -s ../../global/admin/config4.phtml
ln -s ../../global/admin/config5.phtml
ln -s ../../global/admin/delcat.phtml
ln -s ../../global/admin/delitem.phtml
ln -s ../../global/admin/deltemplate.phtml
ln -s ../../global/admin/discassign.phtml
ln -s ../../global/admin/disccodes.phtml
ln -s ../../global/admin/head.phtml
ln -s ../../global/admin/home.phtml
ln -s ../../global/admin/img
ln -s ../../global/admin/index.phtml
ln -s ../../global/admin/invredir.phtml
ln -s ../../global/admin/modcat.phtml
ln -s ../../global/admin/moditem.phtml
ln -s ../../global/admin/moditemdyn.phtml
ln -s ../../global/admin/modmodel.phtml
ln -s ../../global/admin/modshiplocs.phtml
ln -s ../../global/admin/news.phtml
ln -s ../../global/admin/scalecodes.phtml
ln -s ../../global/admin/scaleranges.phtml
ln -s ../../global/admin/shiptypes.phtml
ln -s ../../global/admin/shipzones.phtml
ln -s ../../global/admin/viewcats.phtml
ln -s ../../global/admin/viewitems.phtml
ln -s ../../global/admin/viewreceipt.phtml
ln -s ../../global/admin/viewtemplate.phtml
ln -s ../../global/admin/viewtrans.phtml
ln -s ../../global/admin/viewzone.phtml
cp    ../../default/admin/.htaccess .

echo "Creating include scripts..."
cd $STOREFRONT/$STOREID/inc
ln -s ../../global/inc/checkout_html_finish.inc
ln -s ../../global/inc/checkout_html_footer.inc
ln -s ../../global/inc/checkout_html_form1.inc
ln -s ../../global/inc/checkout_html_form2.inc
ln -s ../../global/inc/checkout_html_header.inc
ln -s ../../global/inc/checkout_receipt.inc
ln -s ../../global/inc/checkout_required.inc
ln -s ../../global/inc/classes.inc
ln -s ../../global/inc/functions.inc
ln -s ../../global/inc/global_config.inc
ln -s ../../global/inc/html.inc
cp    ../../default/inc/config.inc .

echo "Setting file permissions..."
chown -R httpd:httpd $STOREFRONT/$STOREID
chmod -R 600 $STOREFRONT/$STOREID

echo "Setting directory permissions..."
chmod 100 $STOREFRONT/$STOREID
chmod 100 $STOREFRONT/$STOREID/admin
chmod 100 $STOREFRONT/$STOREID/admin/img
chmod 100 $STOREFRONT/$STOREID/inc
chmod 700 $STOREFRONT/$STOREID/cache

cd $DYNDIR
echo "Creating templates directory and copying default templates..."
mkdir templates
cp $STOREFRONT/default/templates/* $DYNDIR/templates
echo "Linking to dyn structure from specified directory path..."
ln -s $STOREFRONT/$STOREID dyn
cd $STOREFRONT

echo "Creating SQL import file..."
echo "This doesn't work... you'll need to delete the db and re-add it"
#sed -e 's/%STOREID%/$STOREID/' sql/blank.mysql > sql/$STOREID.mysql
#$MYSQL/bin/mysql -p < sql/$STOREID.mysql 
echo "Done."

