<script src="/jquery.js"></script>
<script>

function raspi_play() {
	var self = $(this);
	var val = self.attr('value');
	alert(val);
}

$( document ).ready(function() {

    $('a.raspi').click(function(event) {
	event.preventDefault();
        var obj = $(this);
        var uri = obj('uri');
        $.get(uri);
    });

}

</script>
 

<?php

function set_imgsize($s) {
	global $imgsize;
	$imgsize = $s;
}

function set_galleryimg($s) {
	global $gallery_img;
	$gallery_img = $s;
}

function set_origlink() {
	global $origlink;
	$origlink = true;
}

function get_file_uri_prefix($f) {
    if (file_exists('.video_ls')) {
        $prefix = file_get_contents('.video_ls');
        return "$prefix/$f";
    } elseif (file_exists('.video_ls')) {
        $prefix = file_get_contents('.nfs_uripath');
        return "$prefix/$f";
    } else {
        return $f;
    }
}

function link_to_orig($uri,$label,$f_type='',$f_path='') {
	global $origlink;
	if ($origlink) {
	    if (array_key_exists('s', $_REQUEST)) {
	        echo "<div class='picture left' style='width:502px;'>";
	    } elseif (array_key_exists('slides', $_REQUEST)) {
	        echo "<div class='picture left' style='width:502px;'>";
	    } elseif (array_key_exists('large', $_REQUEST)) {
	        echo "<div class='picture left' style='width:1002px;'>";
	    } else {
	        echo "<div class='picture left' style='width:202px;'>";
	    }
            if ($f_type == 'nfs') {
	        printf("<a target='_blank' href='%s'>", $f_path);
            } else {
	        echo "<a target='_blank' href='$uri'>";
            }
	    echo $label;
	    echo "</a>";
	    if (!array_key_exists('v', $_REQUEST) and 
	      !array_key_exists('s', $_REQUEST)) {
		printf("<br /><div align='center'>%.28s</div>", $uri);
		if (strlen($uri) > 28) {
			printf('&hellip;');
		}
		if (is_dir('.meta') and file_exists(".meta/$uri")) {
		    $title = preg_grep("/^title=/", file(".meta/$uri"));
		    foreach ($title as $t) {
			$t2 = preg_replace('/^title=/', '', $t);
		    	printf('<br /><div align="center">');
			printf('<font color="green">%.32s</font></div>', $t2);
			if (strlen($t2) > 32) {
			    printf('&hellip;');
			}
		    }
		}
		printf("<br /><a href='index.php?v=$uri'>IPTC info</a>");
		if (is_dir('.slides')) {
		    printf(" :: <a href='index.php?s=%s'>slide</a>", $uri);
		}
		if (is_dir('.large')) {
		    printf(" :: <a href='index.php?l=%s'>large</a>", $uri);
		}
		if (pdf_name($uri, 'pdf')) {
		    printf(" :: <a target='_blank' href='%s'>PDF</a>", 
			pdf_name($uri, 'pdf'));
		}
		if (pdf_name($uri, 'tif')) {
		    printf(" :: <a target='_blank' href='%s'>TIFF</a>", 
			pdf_name($uri, 'tif'));
		}
		if (pdf_name($uri, 'tiff')) {
		    printf(" :: <a target='_blank' href='%s'>TIFF</a>", 
			pdf_name($uri, 'tif'));
		}
		if (pdf_name($uri, 'epub')) {
		    printf(" :: <a target='_blank' href='%s'>DJVU</a>", 
			pdf_name($uri, 'epub'));
		}
		if (pdf_name($uri, 'mobi')) {
		    printf(" :: <a target='_blank' href='%s'>DJVU</a>", 
			pdf_name($uri, 'mobi'));
		}
		if (pdf_name($uri, 'djvu')) {
		    printf(" :: <a target='_blank' href='%s'>DJVU</a>", 
			pdf_name($uri, 'djvu'));
		}
	    }
	    echo "</div>";
	} else {
		echo $label;
	}
}

function get_title($dir,$default = "UNTITLED") {
	global $title;
        if (file_exists('.title')) {
                if ($fp = fopen('.title','r')) {
                        $buf = fgets($fp);
                        fclose($fp);
                        $title = $buf;
                } else {
                        $title = $default;
                }
        } else {
                $title = $default;
        }
}

function load_asset($a) {
    $h = array();
    if (file_exists($a)) {
        if ($fp = fopen($a,'r')) {
            while (($buf = fgets($fp, 4096)) !== false) {
                if (preg_match('/^#/', $buf)) {
                    $junk = 1;
                } elseif (preg_match('/^$/', $buf)) {
                    $junk = 1;
                } elseif (preg_match('/^\s*$/', $buf)) {
                    $junk = 1;
                } elseif (preg_match('/(?P<tag>\w+)=(?P<val>.*)/', $buf, $m)) {
                    if ($m['val'] != '') $h[$m['tag']] = $m['val'];
                }
            }
            fclose($fp);
        }
    }
    return $h;
}

function is_image($f) {
        $suf= array('jpg', 'jpeg', 'jp2', 'png', 'gif');
        if (is_file($f)) {
		if ($f === '.thumb.png') return false;
		if (preg_match("/^\._/i", $f)) return false;
		if (file_exists('.image_files') and 
		    preg_grep("/^$f\$/", file('.image_files'))) {
			return true;
		}
                foreach ($suf as $suffix) {
                        if (preg_match("/\.$suffix\$/i", $f)) return true;
                }
        }
        return false;
}

function is_video($f) {
	$suf = explode('|',
		'avi|mp4|mpg|mpeg|mov|wmv|m4v|mkv|webm|flv|rm|rmvb|ty|tivo');

        /* if (is_file($f)) { */
		if (preg_match("/^\._/i", $f)) return false;
                foreach ($suf as $suffix) {
                        if (preg_match("/\.$suffix\$/i", $f)) return true;
                }
        /* } */
        return false;
}

function is_media($f) {
	$suf = explode('|', 'vhs|dvd|bluray');

        if (is_file($f)) {
		if (preg_match("/^\._/i", $f)) return false;
                foreach ($suf as $suffix) {
                        if (preg_match("/\.$suffix\$/i", $f)) return true;
                }
        }
        return false;
}

function is_audio($f) {
	$suf = explode('|',
		'mp3|ogg|wav|flac|m4a|aac|aiff|m3u');

        if (is_file($f)) {
		if (preg_match("/^\._/i", $f)) return false;
                foreach ($suf as $suffix) {
                        if (preg_match("/\.$suffix\$/i", $f)) return true;
                }
        }
        return false;
}

function is_evince($f) {
	$suf = explode('|',
		'pdf|ps|tiff|tif|dvi|djvu|cbr|cbz|cb7');

        if (is_file($f)) {
		if (preg_match("/^\._/i", $f)) return false;
                foreach ($suf as $suffix) {
                        if (preg_match("/\.$suffix\$/i", $f)) return true;
                }
        }
        return false;
}

function is_office($f) {
    $suf = explode('|',
	'doc|docx|xls|xlsx|ppt|pptx|odt|odm|ods|odg|odp|odf|odb|oxt' .
	'|xcf|pdf|djvu|txt|eml|tex|cbri|mobi|epub|lit|azw|tpz|azw1' .
	'|rtf|pdb|cbr|cbz|ini');

    if (is_file($f)) {
	if (preg_match("/^\._/i", $f)) return false;
	    foreach ($suf as $suffix) {
		if (preg_match("/\.$suffix\$/i", $f)) return true;
	    }
	}
    return false;
}

function is_asset($f) {
    $suf = explode('|', 'asset|ini');

    if (is_file($f)) {
	if (preg_match("/^\._/i", $f)) return false;
	    foreach ($suf as $suffix) {
		if (preg_match("/\.$suffix\$/i", $f)) return true;
	    }
	}
    return false;
}

function is_webloc($f) {
        if (is_file($f)) {
                if (preg_match("/\.webloc\$/i", $f)) return true;
                elseif (preg_match("/\.url\$/i", $f)) return true;
                elseif (preg_match("/\.desktop\$/i", $f)) return true;
        }
        return false;
}

function has_title($f) {
        global $f_title;
        if (is_dir($f)) {
                if (file_exists($f . '/.title')) {
                        $dir_title = file_get_contents($f . '/.title');
                        $f_title[$f] = $dir_title;
                        return true;
                }
        }
        return false;
}

function pdf_name($f,$pdf_suffix) {
        $suf= array('jpg', 'JPG', 'jpeg', 'jp2', 'png', 'gif');
	$cap = strtoupper($pdf_suffix);
	foreach ($suf as $s) {
		$pdf = basename($f, ".$s") . '.' . $pdf_suffix;
		$cap_pdf = basename($f, ".$s") . '.' . $cap;
        	if (file_exists($pdf)) return $pdf;
        	if (file_exists($cap_pdf)) return $cap_pdf;
	}
	return 0;
}

function get_info() {
        if (file_exists('.info')) { return file_get_contents('.info'); }
        else { return ''; }
}

function get_desc() {
        if (file_exists('.desc')) { return file_get_contents('.desc'); }
        else { return ''; }
}

function get_webloc($f) {
	$label = '';
	$val = '';
        if (file_exists($f)) {
            if (preg_match("/\.webloc\$/i", $f)) {
                $str = preg_grep('/<string>/', file($f));
                $val = array_values($str);
                $val = preg_replace('/.*<string>/', '', $val[0]);
                $val = preg_replace('/<\/string>.*/', '', $val);
                $label = preg_replace('/\.webloc$/', '', $f);
		if (preg_match('/^\d+\.\d+\./', $label)) {
			$label = preg_replace('/^\d+\.\d+\./', '', $label);
		} elseif (preg_match('/\.\d+\.\d+$/', $label)) {
			$label = preg_replace('/\.\d+\.\d+$/', '', $label);
		}
            } elseif (preg_match("/\.url\$/i", $f)) {
                $str = preg_grep('/^URL=/i', file($f));
                $val = array_values($str);
                $val = preg_replace('/^URL=/i', '', $val[0]);
                $label = preg_replace('/\.url$/i', '', $f);
            } elseif (preg_match("/\.desktop\$/i", $f)) {
                $str = preg_grep('/^URL=/i', file($f));
                $val = array_values($str);
                $val = preg_replace('/^URL=/i', '', $val[0]);
                $label = preg_replace('/\.url$/i', '', $f);
	    }
            return array(rtrim($val), $label);
        }
        else { return ''; }
}

function page_nav($page_no,$imgs_per_page,$imgs,$type='thumbs') {
	if (count($imgs) < 1) return;

	$num_pages = (int) (count($imgs) / $imgs_per_page);
	if (count($imgs) % $imgs_per_page > 0) {
		$num_pages++;
	}
	if ($page_no > $num_pages) $page_no = $num_pages;

	echo '<div align="right">' . "\n";
	if ($type == 'slides') {
		echo '[<a href="index.php">thumbs</a]> :: ';
		if (is_dir('.large')) {
			echo '[<a href="index.php?large=1">large</a>] :: ';
		}
	} elseif ($type == 'large') {
		echo '[<a href="index.php">thumbs</a>] :: ';
		if (is_dir('.slides')) {
			echo '[<a href="index.php?slides=1">slides</a>] :: ';
		}
	} elseif (is_dir('.slides')) {
		echo '[<a href="index.php?slides=1">slides</a>] :: ';
		if (is_dir('.large')) {
			echo '[<a href="index.php?large=1">large</a>] :: ';
		}
	}
	echo "Page <strong>$page_no</strong> of <strong>$num_pages</strong> &bull;\n";
	echo count($imgs) . " images\n";

	if ($num_pages == 1) {
		echo "</div>\n";
		return;
	} else {
		echo " &bull; ";
	}

	# first page
	echo $page_no == 1 ? grey_page('1') : page_link(1, $type);

	# prev pages
	if ($page_no < 3) {
	} elseif ($page_no < 6) {
		foreach (range(2, $page_no - 1) as $num) {
			echo ", " . page_link($num, $type);
		}
	} else {
		echo ' ... ' . page_link($page_no - 3, $type);
		foreach (range($page_no - 2, $page_no - 1) as $num) {
			echo ", " . page_link($num, $type);
		}
	}

	# current page
	if ($page_no > 1 and $page_no < $num_pages) {
		echo ", ";
		grey_page($page_no);
	}

	# next pages
	if ($page_no > $num_pages - 2) {
		echo ", ";
	} elseif ($page_no < $num_pages - 4) {
		foreach (range($page_no + 1, $page_no + 3) as $num) {
			echo ", " . page_link($num, $type);
		}
		echo " ... ";
	} else {
		foreach (range($page_no + 1, $num_pages - 1) as $num) {
			echo ", " . page_link($num, $type);
		}
		echo ", ";
	}

	# last page
	echo $page_no == $num_pages ? grey_page($num_pages) : 
		page_link($num_pages, $type);
	echo "</div>\n";
}

function page_link($page_no,$type='thumbs') {
	$uri = 'index.php';
	$full_uri = $page_no == 1 ? $uri : "$uri?page=$page_no";
	if ($type == 'slides') {
		if ($page_no == 1) {
			$full_uri .= '?slides=1';
		} else {
			$full_uri .= '&slides=1';
		}
	} elseif ($type == 'large') {
		if ($page_no == 1) {
			$full_uri .= '?large=1';
		} else {
			$full_uri .= '&large=1';
		}
	}
	return "<a href='$full_uri'>[$page_no]</a>";
}

function grey_page($param) {
	echo "<font color='$greycolor'>[" .  $param .  ']</font>'; 
}

function fix_vid_path($param) {
	$repos = file('/var/www/html/gt_video/REPOS.txt');
	$repolist = array();
	foreach ($repos as $repo) {
		list($tag,$val) = preg_split('/ -> /', $repo);
		$repolist[$tag] = rtrim($val);
	}
	$vid_path = preg_replace('/^\/video\//', '', $param);
	list($repo_name,$vid_path) = preg_split('/\//', $vid_path, 2);
	return 'file://' . $repolist[$repo_name] . '/' . $vid_path;
}

function vid_file_exists($f, $vid_path) {
	$dir = preg_replace('/^file:\/\//', '', $vid_path);
	if (file_exists($dir . '/' . $f)) {
		return 1;
	} else {
		printf("<p>NO SUCH FILE {%s} in dir {%s}</p>\n", $f, $dir);
		return 0;
	}
}

function handle_bbcode($text) {
	require_once($_SERVER['DOCUMENT_ROOT'] . "/nbbc.php");
	$bbcode = new BBCode;
	$bbcode->SetSmileyURL('/smileys');

	$bbcode->AddSmiley(':moof:', 'moof.gif');
	$bbcode->AddSmiley(':apple:', 'apple.gif');
	$bbcode->AddSmiley(':dogcow:', 'dogcow-flip.gif');
	$bbcode->AddSmiley(':macplus:', 'happy-mac.gif');

	### $gtscity_icons = explode(' ', 'badjoke bigcry censored dead facepalm headscratch inlove lol popcorn rage rant redface rock rolleyes sleep thumbsup yikes zombie');
	### foreach ($gtscity_icons as $gts) {
		### $bbcode->AddSmiley(':'. $gts . ':', 'icon_' . $gts . '.gif');
	### }

	$bbcode->AddRule('youtube', Array('simple_start' => '<iframe 
	    title="YouTube player" class="youtube-player" type="text/html" 
	    width="240" height="140" src="http://www.youtube.com/embed/',
	    'simple_end' => '" frameborder="0" allowFullScreen></iframe>',
	    'class' => 'inline',
	    'allow_in' => Array('block', 'columns', 'inline', 'link'),
	  )
	);

	return $bbcode->Parse($text);
}

function raspi_play($uri) {
  $service_port = 8888;
  $address = gethostbyname('sawtooth.abreau.net');

  $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
  if ($socket) {
    $result = socket_connect($socket, $address, $service_port);
    if ($result) {
      $in = "PLAY " + $uri + "\n";
      socket_write($socket, $in, strlen($in));
      socket_close($socket);
    }
  }
}


$images = array();
$videos = array();
$audios = array();
$weblocs = array();
$offices = array();
$assets = array();
$dirs = array();
$files = array();
$metadata = array();


if ($dh = opendir(".")) {
    while ($f = readdir($dh)) {
        if (has_title($f)) {
            if (preg_match('/^\./', $f) == false) {
                $dirs[] = $f;
            }
        } elseif ( preg_match("/^\._/", $f) ) {
		/* skip */
        } elseif ( preg_match("/^\.foo\.jpg$/", $f) ) {
		/* skip */
        } elseif ( preg_match("/^index\.php$/", $f) ) {
		/* skip */
        } elseif ( preg_match("/^\.gallery\.jpg$/", $f) ) {
		$gallery_img = '.gallery.jpg';
        } elseif ( preg_match("/^gallery\.jpg$/", $f) ) {
		$gallery_img = 'gallery.jpg';
        } elseif (is_image($f)) {
            $images[] = $f;
        } elseif (is_webloc($f)) {
            $weblocs[] = $f;
        } elseif ( preg_match("/^\./", $f) ) {
		/* skip */
        } elseif (is_video($f) || is_media($f)) {
            $videos[] = $f;
        } elseif (is_audio($f)) {
            $audios[] = $f;
        } elseif (is_asset($f)) {
            $assets[] = $f;
        } elseif (is_office($f)) {
            $offices[] = $f;
        } elseif (is_file($f)) {
	    $files[] = $f;
        }
    }
}

if (file_exists('.docmgr')) {
	$images = array_merge($images, $offices);
}

if (file_exists('.asset_tracker')) {
	$images = array_merge($images, $assets);
}

if (file_exists('.reverse')) {
	rsort($images);
} else {
	asort($images);
}

asort($weblocs);
asort($dirs);
asort($videos);
asort($audios);
asort($offices);
asort($assets);
asort($files);

set_origlink();
?>

<style type="text/css">
ul.dir {list-style:none;}
ul.dir > li {
        margin:0 0 .1em;
        background:url(data:image/gif;base64,R0lGODlhEAAOALMAAOazToeHh0tLS/7LZv/0jvb29t/f3//Ub//ge8WSLf/rhf/3kdbW1mxsbP//mf///yH5BAAAAAAALAAAAAAQAA4AAARe8L1Ekyky67QZ1hLnjM5UUde0ECwLJoExKcppV0aCcGCmTIHEIUEqjgaORCMxIC6e0CcguWw6aFjsVMkkIr7g77ZKPJjPZqIyd7sJAgVGoEGv2xsBxqNgYPj/gAwXEQA7)
top left no-repeat; )
        height:14px;
        text-indent:1.5em;
}

/* OLD: whitish background
.picture {
    background-color: #F9F9F9;
    border: 1px solid #CCCCCC; padding: 3px;
    font: 11px/1.4em Arial, sans-serif;
}
.picture img {
    border: 1px solid #CCCCCC;
    vertical-align: middle;
    margin-bottom: 3px;
}
*/

.picture {
    background-color: transparent;
    border: 0 padding: 3px;
    font: 11px/1.4em Arial, sans-serif;
}
.picture img {
    border: 0 ;
    vertical-align: middle;
    margin-bottom: 3px;
}
.picture h3 {
    text-align:center;
}


.right { margin: 0.5em 0pt 0.5em 0.8em; float: right; }
.left { margin: 0.5em 0.8pt 0.5em 0em; float: left; }

/* BEGIN - nbbc - bbcode styling */

/* Add a dashed underline to acronyms. */
span.acronym { border-bottom:1px dashed green; }
span.acronym:hover { color: green; border-bottom:1px dashed lightgreen; }

/* Make spoilers invisible, so that you need to select them with the mouse. */
span.spoiler { background-color: black; color: black; }

/* Align columns to the top, and add some space between them. */
table.bbcode_columns { border-collapse: collapse;
    margin-top: 1em; margin-bottom: 1em; }
table.bbcode_columns td.bbcode_column { padding: 0 1em; vertical-align: top;}
table.bbcode_columns td.bbcode_firstcolumn { border-left: 0; padding-left: 0; }

/* Wrap quotes in a big blue box. */
div.bbcode_quote { border: 1px transparent; margin: 0.5em 0; }
div.bbcode_quote_head { background-color: burlywood; 
    font-weight: bold; padding: 0.25em 0.5em; }
div.bbcode_quote_head a:link { color: red; }
div.bbcode_quote_head a:visited { color: yellow; }
div.bbcode_quote_head a:hover { color: white; text-decoration: underline; }
div.bbcode_quote_head a:active { color: white; text-decoration: underline; }
div.bbcode_quote_body { background-color: burlywood;
    color: black; padding: 0.5em 1em; }

/* Wrap code in a big blue box. */
div.bbcode_code { border: 1px solid blue; margin: 0.5em 0; }
div.bbcode_code_head { background-color: blue; color: white;
    font-weight: bold; padding: 0.25em 0.5em; }
div.bbcode_code_body { background-color: skyblue; color: black;
    font: 10pt monospace; padding: 0.5em 1em; }

/* END - nbbc - bbcode styling */
</style>

<?php include 'logo.inc' ?>

<?php
$my_path = explode('/', $_SERVER['SCRIPT_NAME']) ;
array_shift($my_path);
array_pop($my_path);
array_pop($my_path);
?>

<?php $separator = '&gt;' ?>
<a href="/">root</a>
<?php for ($n = 0; $n < count($my_path) ; $n++) { ?>
	<?= $separator ?>
	<a href="/<?= implode('/', array_slice($my_path, 0, 1 + $n)) ?>/">
	<?= $my_path[$n] ?></a>
<?php } ?>

<?php if (file_exists('.title')) { ?>
	<?php get_title('.'); ?>
	<h1><?= $title ?></h1>
<?php } ?>

<?php if (file_exists('.asset_tracker')) { ?>
	<div><form name="form">
	<button type="button">New Asset...</button>
	</form></div>
<?php } ?>

<?php if (file_exists('.reverse')) { rsort($dirs); } ?>
<?php if (file_exists('.dirs_as_folder_list')) { ?>
<ul class="dir">
<?php foreach ($dirs as $dir) { ?>
	<li><a href="./<?= $dir ?>"><?= $f_title[$dir] ?></a></li>
<?php } ?>
</ul>
<?php } else { ?>
<p>[ 
<?php $separator = '' ?>
<?php foreach ($dirs as $dir) { ?>
    <?= $separator ?>
    <?php $separator = '|' ?>
    <a href="./<?= $dir ?>"><?= $f_title[$dir] ?></a>
<?php } ?>
 ]</p>
<?php } ?>


<hr />

<?php $dir_thumbs = glob('*/.thumb.png'); ?>
<?php if (!empty($dir_thumbs)) { ?>
	<p>
	<?php if (file_exists('.reverse')) { rsort($dir_thumbs); } ?>
	<?php foreach ($dir_thumbs as $th) { ?>
		<?php $th_dir = preg_replace('/\/\.thumb.png/', '', $th); ?>
		<?php if (has_title($th_dir)) { ?>
			<?php $th_title = $f_title[$th_dir]  ?>
		<?php } else { ?>
			<?php $th_title = $th_dir  ?>
		<?php } ?>
		<?php if (strlen($th_title) > 20) { ?>
			<?php $th_short_title = substr($th_title, 0, 20) . '...' ?>
		<?php } else { ?>
			<?php $th_short_title = $th_title ?>
		<?php } ?>
		<div class='picture left' style='width:202px;'>
		    <a target="_blank" href="<?= $th_dir ?>">
		        <img title="<?= $th_title ?>" src="<?= $th ?>" />
		    </a>
		    <br />
		    <div align="center">
		        <font size="+1"><?= $th_short_title ?></font>
		    </div>
		</div>
	<?php } ?>
	</p>
	<br clear='all'>
	<hr />
<?php } ?>


<?php if (count($weblocs)) { ?>
        <ol>
        <?php foreach ($weblocs as $webloc) { ?>
                <?php $loc = get_webloc($webloc); ?>
                <li><a target="_blank"  href="<?= $loc[0] ?>">
                        <?= $loc[1] ?></a></li>
        <?php } ?>
        </ol>
        <hr />
<?php } ?>

<?php
        $desc = get_desc();
        if ($desc != '') {
                echo '<p>' . $desc . '</p><hr />';
        }

        if (file_exists('.listfiles')) {
		echo '<ul>';
		foreach ($files as $f) {
			echo '<li>' . $f . '</li>';
		}
		echo '</ul>';
	}

        if (file_exists('.linkfiles')) {
		echo '<p><strong>.linkfiles</strong></p>';
		echo '<ul>';
		foreach ($files as $f) {
			echo '<li>';
			echo '<a target="_blank" href="' . $f . '">';
			echo $f . '</a>';
			echo '</li>';
		}
		echo '</ul>';
	}

        if (file_exists('.inline')) {
		include '.inline';
	}

        if (file_exists('.include')) {
		print handle_bbcode(file_get_contents('.include'));
	}

        if (file_exists('.iframe')) {
                if ($fp = fopen('.iframe','r')) {
                        $buf = fgets($fp);
                        fclose($fp);
                        $iframe_uri = $buf;
                } else {
                        $iframe_uri = 'http://127.0.0.1/';
                }
		printf('<iframe width="100%%" height="1000" src="%s">', $iframe_uri);
		printf("if you see this, then your browser didn't process the iframe tags correctly.");
	}

	if (count($audios) > 0) {
		$nfs_uripath = file_get_contents('.nfs_uripath');
		printf("<ul>\n");
		foreach ($audios as $audio) {
			if ($nfs_uripath != '') {
				printf('<li><a href="%s/%s">%s</a></li>', 
					$nfs_uripath, $audio, $audio);
			} else {
				printf('<li>%s</li>', $audio);
			}
		}
		printf("</ul>\n");
	}

	if (!isset($imgsize)) {
		if (array_key_exists('s', $_REQUEST)) {
			$imgsize = 500;
		} elseif (array_key_exists('slides', $_REQUEST)) {
			$imgsize = 500;
		} elseif (array_key_exists('large', $_REQUEST)) {
			$imgsize = 1000;
		} else {
			$imgsize = 200;
		}
	}

	if (array_key_exists('v', $_REQUEST)) {
	    $f = $_REQUEST['v'];
	    $exif = exif_read_data($f, 'IFD0');
	    $exif && $exif = exif_read_data($f, 0, true);
	    $desc = $exif['IFD0']['ImageDescription'];
	    $desc_html = preg_replace('/\n/', '<br />', $desc);
	    echo "<h2>" . $f . "</h2>";
	    link_to_orig($f,
	    	"<img width=$imgsize height=$imgsize 
	    	src=\".thumbs/$f\" title=\"$f\" border=2 />");
	    echo "<br clear='all'>";
	    echo "<p>" . $desc_html . "</p>";
	    echo "<hr />";
	    $info = preg_grep("/^$f ---- /", file('gallery.dat'));
	    echo "<hr />";

	    // show exif info
	    if ($exif) {
	        echo "<h3>EXIF Info</h3>";
		foreach ($exif as $key => $section) {
		    foreach ($section as $name => $val) {
			echo "$key.$name :: $val<br />\n";
		    }
		}
	    }
        } elseif (array_key_exists('tag', $_REQUEST)) {
                $f = $_REQUEST['tag'];
                echo "<h2>" . $f . "</h2>";
                link_to_orig($f,
                        "<img width=$imgsize height=$imgsize
                        src=\".thumbs/$f\" title=\"$f\" border=2 />");
                /* echo "<br clear='all'>"; */
                echo '<form method="POST">';

		echo '<table border=0 cellspacing=3 cellpadding=3>';
		echo '<tr><th valign=top>People</th>';
                echo '<td valign=top><textarea name="people_text" cols="40" rows="6">';
                echo '</textarea></td></tr>';

                echo '<th valign=top>Tags</th>';
                echo '<td valign=top><textarea name="tags_text" cols="40" rows="6">';
                echo '</textarea></td></tr>';

                echo '<tr><th>&nbsp;</th><td><input type="submit" name="button" value="Save" /></td></tr>';
                echo '</form>';
		echo '</table>';
                echo "<br clear='all'>"; 
                echo "<hr />";
	} elseif (array_key_exists('s', $_REQUEST)) {
	    $f = $_REQUEST['s'];

            if (file_exists(".meta/$f")) {
		$meta = preg_grep("/(\w+)=(.*)/", file(".meta/$f"));
		foreach ($meta as $pair) {
		    list($key,$val) = preg_split("/=/", $pair);
		    $metadata[$key] = $val;
		}
	    } elseif (file_exists("gallery.dat")) {
	        $title = preg_grep("/^$f ---- /", file('gallery.dat'));
	        if ($title != '') {
	            $title = preg_replace("/^$f ---- /", "", $title);
		    $metadata['title'] = $title;
	        }
	    }

	    echo "<h2><font color='green'>".$metadata['title']."</font></h2>";

	    link_to_orig($f,
	    	"<img width=$imgsize height=$imgsize 
	    	src=\".slides/$f\" title=\"$f\" border=2 />");
	    echo "<br clear='all'>";

	    if (array_key_exists('description', $metadata)) {
		echo "<hr />";
		foreach (explode('\n', $metadata['description']) as $p) {
		    echo "<p><font color='green'>" . $p . "</font></p>\n";
		}
	    }

	} elseif (array_key_exists('pdf', $_REQUEST)) {
	    $f = $_REQUEST['s'];

            if (file_exists(".meta/$f")) {
		$meta = preg_grep("/(\w+)=(.*)/", file(".meta/$f"));
		foreach ($meta as $pair) {
		    list($key,$val) = preg_split("/=/", $pair);
		    $metadata[$key] = $val;
		}
	    } elseif (file_exists("gallery.dat")) {
	        $title = preg_grep("/^$f ---- /", file('gallery.dat'));
	        if ($title != '') {
	            $title = preg_replace("/^$f ---- /", "", $title);
		    $metadata['title'] = $title;
	        }
	    }

	    echo "<h2><font color='green'>".$metadata['title']."</font></h2>";

	    echo "<h1><font color='red'>PDF inline goes here</font></h1>";
	    echo "<br clear='all'>";

	    if (array_key_exists('description', $metadata)) {
		echo "<hr />";
		foreach (explode('\n', $metadata['description']) as $p) {
		    echo "<p><font color='green'>" . $p . "</font></p>\n";
		}
	    }

	    echo "<hr />";
	    echo '<p><strong>metadata</strong></p>';
	    echo "<ul>\n";
	    foreach ($metadata as $key => $val) {
	        if (preg_match("/\|/", $val)) {
	            echo "<li><strong>".$key."</strong> :: ";
		    echo implode(" &bull; ", preg_split("/\|/", $val));
		    echo "</li>";
	        } else {
	            echo "<li><strong>".$key."</strong> :: ".$val."</li>";
	        }
	    }
	    echo "</ul>\n";

        } elseif (!file_exists('.hide_images')) {
		if (array_key_exists('slides', $_REQUEST)) {
			$imgs_per_page = 20;
			$dir_type = 'slides';
		} elseif (array_key_exists('large', $_REQUEST)) {
			$imgs_per_page = 10;
			$dir_type = 'large';
		} else {
			$imgs_per_page = 100;
			$dir_type = 'thumbs';
		}
		$page_no = array_key_exists('page', $_REQUEST) ? 
			$_REQUEST['page'] : 1;
		page_nav($page_no, $imgs_per_page, $images, $dir_type);

		$slice = array_slice($images, ($page_no - 1) * $imgs_per_page, 
			$imgs_per_page);
		foreach ($slice as $f) {
		    if (file_exists('.docmgr') && is_office($f)) {
			$ff = preg_replace('/\.[^.]+$/', '.png', $f);
			$fu = get_file_uri_prefix($f);
			link_to_orig($f, "<img width=$imgsize height=$imgsize 
			    src=\".$dir_type/$ff\" title=\"$f\" border=2 />",
			    "nfs", $fu);
		    } else {
			link_to_orig($f, "<img width=$imgsize height=$imgsize 
			    src=\".$dir_type/$f\" title=\"$f\" border=2 />");
		    }
		}
		echo "<br clear='all'>";

		page_nav($page_no, $imgs_per_page, $images, $dir_type);
	}

        if (file_exists('.video_ls') || file_exists('.nfs_uripath')) {
		$vid_prefix = preg_replace('/[\r\n]$/', '',
			file_get_contents('.video_ls') || 
			file_get_contents('.nfs_uripath'));
		## $vid_path = fix_vid_path($_SERVER['REQUEST_URI']); 
		$vid_path = $vid_prefix;

		$vidlist = array();
		$vidtype = 'unknown';
		if (file_exists('.index-2')) {
		    $vids = file('.index-2');
		    foreach ($vids as $vid) {
        		if (preg_match('/^#/', $vid)) {
                		/* skip */
        		} elseif (preg_match('/^$/', $vid)) {
                		/* skip */
        		} elseif (preg_match('/\|/', $vid)) {
                		$h = explode('|', $vid);
				if ($h[1] == 'movie') {
					$vidtype = 'movies';
				} elseif ($h[1] == 'tv-series') {
					$vidtype = 'tv';
				} elseif ($h[1] == 'home') {
					$vidtype = 'home';
				} else {
					$vidtype = 'tv';
				}
				if (vid_file_exists($h[0], $vid_path)) {
                			$vidlist[] = $h;
				}
        		}
		    }
		    asort($vidlist);
		}
		if ($vidtype == 'tv') {
        		echo '<ul>';
		} elseif ($vidtype == 'home') {
        		echo '<ul>';
		} else {
        		echo '<p>';
		}

		$movie_cover_dir = '/video/covers/.thumbs';
		foreach ($vidlist as $vid) {
        	    if ($vidtype == 'tv') {
                	printf('<li>');
			$pi_vid_prefix = preg_replace('/^file:\/\/\//',
			    '/pi-play.php/', $vid_prefix);
                	printf('<button type="button"
			    onclick="$.get(\'%s/%s\')">',
			    $pi_vid_prefix, $vid[0]);
			printf('Play</button> : ');
                	printf('%s %s - ', $vid[2], $vid[3]);
			printf('<a target="_blank" href="%s/%s">', $vid_prefix, $vid[0]);
			printf('%s</a>', $vid[6]);
                	printf('(%s) :: [%s]</li>', $vid[4], $vid[5]);
        	    } elseif ($vidtype == 'movies') {
                	$movie_base = preg_replace('/---.*/', '', 
                       		preg_replace('/\.[^.]*$/', '', $vid[0]));
                	$movie_cover = "$movie_cover_dir/$movie_base.jpg";

			echo "<div class='picture left' style='width:202px;'>";
			if (!is_media($vid[0])) {
				printf('<a target="_blank" href="%s/%s">', 
					$vid_prefix, $vid[0]);
			}
			printf('<img title="%s (%s)" src="%s" />', 
				$vid[4], $vid[2], $movie_cover);
			if (!is_media($vid[0])) {
				print '</a>';
			}
			echo '<br /><div align="center">';

			$pi_vid_prefix = preg_replace('/^file:\/\/\//',
				'/pi-play.php/', $vid_prefix);
			printf('<button type="button" 
				  onclick="$.get(\'%s/%s\')">',
				  $pi_vid_prefix, $vid[0]);

			if (strlen($vid[4]) > 14) {
				printf('%-14.14s&hellip; ', $vid[4]);
			} else {
				printf('%s ', $vid[4]);
			}
			printf('(%s)</button>', $vid[2]);
			echo "\n\n";
			printf('</div></div>');
		    } else {
                	printf('<li>');
			$pi_vid_prefix = preg_replace('/^file:\/\/\//',
			    '/pi-play.php/', $vid_prefix);
                	printf('<button type="button"
			    onclick="$.get(\'%s/%s\')">',
			    $pi_vid_prefix, $vid[0]);
			printf('Play</button>');
                	printf('%s %s - ', $vid[2], $vid[3]);
			printf('<a target="_blank" href="%s/%s">', $vid_prefix, $vid[0]);
			printf('%s</a>', $vid[6]);
                	printf('(%s) :: [%s]</li>', $vid[4], $vid[5]);
		    }
		}
		if ($vidtype == 'tv') {
			echo '</ul>';
		} elseif ($vidtype == 'home') {
			echo '</ul>';
		} else {
			echo '</p>';
		}
		echo '<br clear="all">';

		/* unlisted video files -- not in .index */
		$vidmatch = array();
		foreach ($vidlist as $vid) {
		    $vidmatch[$vid[0]] = 1;
		}
		$vid_new = array();
		foreach ($videos as $v) {
			if (array_key_exists($v, $vidmatch)) {
				/* skip */
			} else {
				$vid_new[] = $v;
			}
		}
		if (count($vid_new) > 0) {
		    echo '<hr /><h3> Unfiled Videos </h3>';
		    echo '<ol>';
		    foreach ($vid_new as $v) {
			$pi_vid_prefix = preg_replace('/^file:\/\/\//', 
			    '/pi-play.php/', $vid_prefix);
			printf('<li>');
			printf('<button type="button" 
				onclick="$.get(\'%s/%s\')"
				>Play</button> : ', $pi_vid_prefix, $v);
			printf('<a target="_blank" href="%s/%s">%s</a>', 
			    $vid_prefix, $v, $v);
			printf('</li>');
		    }
		    echo '</ol>';
		}
	}

        if (file_exists('.asset_tracker')) {
        	echo '<ul>';

		foreach ($assets as $asset) {
                    $ainfo = load_asset($asset);
                    printf('<li>%s :: ', $asset);
		    if (array_key_exists('description', $ainfo))
		        print $ainfo['description'];
		    elseif (array_key_exists('manufacturer', $ainfo)) {
		        printf('(%s', $ainfo['manufacturer']);
		        if (array_key_exists('modelnumber', $ainfo))
		            print $ainfo['modelnumber'];
		        printf(')');
		    }
		    if (array_key_exists('hostname', $ainfo))
		        printf(' [%s]', $ainfo['hostname']);
		    print '</li>';
		}

		echo '</ul>';
		echo '<br clear="all">';
	}

        if (file_exists('.test_thumbs')) {
	    echo '<hr />';
	    echo '<h3> DEBUG: display all .thumbs</h3>';
	    foreach ( glob('.thumbs/*') as $thumb_img ) {
		$thumb_base = preg_replace('/^\.thumbs\//', '', $thumb_img);
		printf('<img title="%s" src="%s" />', $thumb_base, $thumb_img);
	    }
	}


?>

