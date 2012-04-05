<?php
define('NEWLINE', '
');

function finish($buffer) {
	file_put_contents('../documentation/index.html', str_replace('</div>', '</div>
', $buffer));
	return $buffer;
}

ob_start('finish');

require('functions.php');
require('parse_code.php');

$_chapters = array();

?>
<!doctype HTML>
<html>
	<head>
		<title>Simple C++ Game Engine Documentation</title>
		<style>
		<?php include('style.css'); ?>
		</style>
	</head>
	<body>
		<?php
			wip();
			
			$c = new_chapter('Preparations');
			section_from_file($c, 'preparations');
			
			$c = new_chapter('Hello World');
			section_from_file($c, 'helloworld');
			
			$jump = new_chapter('Jump!');
			
			parse_code(array(
				'../engine/audio.cpp',
				'../engine/display.parseme.cpp',
				'../engine/engine.cpp',
				'../engine/extra.cpp',
				'../engine/midi.cpp',
				'../engine/network.cpp',
				'../engine/python.cpp'
			));
			
			add_code_chapters();
			
			$conclusion = new_chapter('Conclusion & The Future');
			section_from_file($conclusion, 'conclusion');
			
			$jump->add_html(code_category_jumps().'<br />'.jump($conclusion));
			
			draw_chapters();
		?>
	</body>
<html>
<?php
//draw chapters
function draw_chapters() {
	global $_chapters;
	foreach($_chapters as $chapter) {
		if(is_object($chapter) && get_class($chapter) == 'Chapter')
			$chapter->draw();
	}
}

//jump
function jump($chapter) {
	return '<a href="#'.$chapter->link.'">'.$chapter->name.'</a>';
}

//wip
function wip() {
	if(file_exists('wip'))
		echo '<br /><div class="wip">'.nl2br(file_get_contents('wip')).'</div>';
}

//new chapter
function new_chapter($name) {
	global $_chapters;
	
	$chapter = new Chapter($name);
	array_push($_chapters, $chapter);
	return $chapter;
}

//section from file
function section_from_file($chapter, $name) {
	if(file_exists(strtolower($name)))
		$chapter->add_section(null, parse_txt_file(strtolower($name)));
}

//parse_txt
function parse_txt($str) {
	$str = explode(NEWLINE, $str);
	$count = count($str);
	$output = '';
	
	$ln = 0;
	foreach($str as $line) {
		$ln++;
		
		$tag = false;
		$start = 0;
		if(substr(ltrim($line), 0, 1) == '[') {
			$find = strpos($line, '[', 0);
			if($find !== false) {
				$find2 = strpos($line, ']', $find + 1);
				if($find2 !== false) {
					$tag = substr($line, $find + 1, $find2 - $find - 1);
					$start = $find2 + 1;
				}
			}
		}
		
		if($tag !== false) {
			$str = substr($line, $start);
			switch($tag) {
			case 'c++':
				$output .= 'C++'.$str.':';
				$output .= '<div class="code">';
				break;
			case 'python':
				$output .= 'Python'.$str.':';
				$output .= '<div class="code">';
				break;
			case '~c++':
			case '~python':
			case '~description':
				$output .= '</div>';
				break;
			case 'description':
				if(trim($str) == '') {
					$output .= '<div class="description">';
					break;
				}
			case 'description':
				$output .= '<span class="'.$tag.'">';
				$output .= $str;
				$output .= '</span><br />';
				break;
			case 'see':
				$output .= 'See also: '.$str;
				break;
			default:
				$output .= '<div class="'.$tag.'">';
				$output .= $str;
				$output .= '</div>';
				break;
			}
		}
		else
			$output .= $line . ($ln != $count ? '<br />' : '');
	}
	
	return $output;
}

function parse_txt_file($name) {
	return parse_txt(file_get_contents($name));
}

ob_end_flush();
?>
