
<hr />

<?php include 'trailer-local.inc' ?>
<?php
	$inc_dir = dirname($_SERVER['DOCUMENT_ROOT'])."/etc/colmat/";
	$inc_f = 'nonesuch.inc';

	if (file_exists($inc_dir.'/'.$inc_f)) {
		include $inc_f;
	}
?>

<hr />

