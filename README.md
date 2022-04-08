# colmat

A collection of PHP include files for displaying images and other media
using a common index.php file where the local server-specific customizations
reside outside the document root.

Red Hat-based Linux distributions use a server root of "/var/www".
Debian-based Linux distributions use a	server root of "/etc/apache2".
Apple MacOS uses a server root of "/Library/WebServer"
Some sites use a server root refix of "/srv/www" and add virtual hosts 
under that: e.g., example.com's server root at "/srv/www/example.com",
with the document and cgi roots beneath that.

If your document root is at /srv/www/example.com/html and your cgi-bin 
is at /srv/www/example.com/cgi-bin, you can place the colmat files at 
/srv/www/example.com/colmat, and then use an index.php file such as 
the following:

<!DOCTYPE html>
<html>
<head>
<?php if (file_exists('.title')) {
        $title = file_get_contents('.title');
} else {
        $title = $_SERVER['HTTP_HOST'];
} ?>

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link type="text/css" rel="stylesheet" media="all" href="/style.css" />
<title><?php echo $title ?></title>

<?php 
        $colmat_root = dirname($_SERVER['DOCUMENT_ROOT']) . "/colmat";
?>
</head>

<?php include "$colmat_root/bgcolor.inc" ?>
<?php if ($bgcolor != "") { ?>
	<body bgcolor="<?php echo $bgcolor ?>">
<?php } else { ?>
	<body bgcolor="#AAAAAA">
<?php } ?>

<?php include "$colmat_root/header.inc" ?>
<?php if (file_exists(".content")) { include ".content" ; } ?>
<?php include "$colmat_root/trailer.inc" ?>

</body>
</html>

