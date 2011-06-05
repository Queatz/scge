<?php
$code_docs = null;

$_categories = array();
$_definitions = array();

function parse_code($files) {
	global $_categories, $_definitions;
	
	$cat = null;
	$name = null;
	$sstr = '';
	
	foreach($files as $file) {
		$f = fopen($file, 'r');
		
		$processing = false;
		$linen = 0;
		$cat = null;
		$multi = false;
		$indent = 0;
		$codeblock = false;
		$absorbed = false;
		$blanks = 0;
		while($l = fgets($f)) {
			$linen++;
			$line = trim($l);
			
			switch(substr($line, 0, 4)) {
			case '/* *':
				if($processing)
					echo 'Error in '.$file.' at line '.$linen.'<br />';
				
				$processing = true;
				$pline = 0;
				
				if(substr($line, 5) != '')
					$cat = substr($line, 5);
				
				if(is_null($cat)) {
					echo 'Error in '.$file.' at line '.$linen.'<br />';
					echo 'No category defined<br />';
				}
				
				$name = null;
				$sstr = '';
				$multi = false;
				$desc = 0;
				$codeblock = false;
				$absorbed = false;
				break;
			case '* */':
				if(!$processing) {
					echo 'Error in '.$file.' at line '.$linen;
					break;
				}
				
				$processing = false;
				
				if(strlen($name) < 1)
					break;
				
				if($linen < 3)
					break;
				
				if($desc == 1)
					$sstr .= '</span>';
				
				if($codeblock)
					$sstr .= '</div>';
				
				if(!isset($_categories[$cat]))
					$_categories[$cat] = new Chapter(ucwords($cat));
				
				$s = new Section($name, $sstr, $multi);
				$_categories[$cat]->add_section($s);
				
				array_push($_definitions, $s->link);
				break;
			default:
				if($processing) {
					if(is_null($name)) {
						if($line == '*')
							$multi = true;
						elseif($line != '')
							$name = $line;
					}
					else {
						if($line == '') {
							$blanks++;
							continue;
						}
							
						if(substr($line, 0, 1) == '#') {
							if($codeblock) {
								$sstr .= '</div>';
								$codeblock = false;
							}
						
							if($desc == 1) {
								$desc = -1;
								$sstr .= '</span>';
							}
							
							if($nindent == 2)
								$sstr .= '</span>';
							
							$sstr .= '<div class="note">'.substr($line, 1).'</div>';
							
							if($nindent == 2)
								$sstr .= '<span class="description">';
						}
						elseif(substr($line, 0, 1) == '!') {
							if($codeblock) {
								$sstr .= '</div>';
								$codeblock = false;
							}
						
							if($desc == 1) {
								$desc = -1;
								$sstr .= '</span>';
							}
							
							if($nindent == 2)
								$sstr .= '</span>';
							
							$sstr .= '<div class="notice">'.substr($line, 1).'</div>';
							
							if($nindent == 2)
								$sstr .= '<span class="description">';
						}
						elseif(substr($line, 0, 4) == 'see:') {
							if($codeblock) {
								$sstr .= '</div>';
								$codeblock = false;
							}
						
							if($desc == 1) {
								$desc = -1;
								$sstr .= '</span>';
							}
							
							if($nindent == 2)
								$sstr .= '</span>';
							
							$sstr .= ($absorbed ? '<br />' : '').'See also: ' . substr($line, 4);
							
							$absorbed = true;
							
							if($nindent == 2)
								$sstr .= '<span class="description">';
						}
						else {
							if(substr($l, 0, 2) == '		')
								$nindent = 2;
							elseif(substr($l, 0, 1) == '	')
								$nindent = 1;
							else
								$nindent = 0;
							
							if($nindent > 0) {
								if($codeblock) {
									$sstr .= '</div>';
									$codeblock = false;
								}
							
								if($desc == 1) {
									$desc = -1;
									$sstr .= '</span>';
								}
							
								if($indent == 0 && $nindent > 0) {
									$sstr .= '<ul>';
								}
								
								if($nindent == 1)
									$sstr .= '<li>';
								
								if($nindent == 2 && $indent == 1) {
									$sstr .= '<span class="description">';
									$blanks = 0;
								}
								
								if($indent == 2 && $nindent == 1)
									$sstr .= '</span>';
								
								$indent = $nindent;
							}
							else {
								if($indent > 0) {
									$sstr .= '</li>';
									$sstr .= '</ul>';
									
									$absorbed = false;
								
									$indent = 0;
								}
								if($desc == 0) {
									$sstr .= '<span class="description">';
									$desc = 1;
									$blanks = 0;
								}
								elseif($line == 'C++') {
									if($desc == 1) {
										$desc = -1;
										$sstr .= '</span>';
									}
								
									if($codeblock) {
										$sstr .= '</div>';
										$codeblock = false;
									}
									$sstr .= ($absorbed ? '<br />' : '').'C++:<div class="code">';
									$codeblock = true;
									$blanks = 0;
									continue;
								}
								elseif($line == 'Python') {
									if($desc == 1) {
										$desc = -1;
										$sstr .= '</span>';
									}
								
									if($codeblock)
										$sstr .= '</div>';
									$sstr .= ($absorbed ? '<br />' : '').'Python:<div class="code">';
									$codeblock = true;
									$blanks = 0;
									continue;
								}
							}
							if($desc == 1 && $blanks > 0)
								$sstr .= str_repeat('<br />', $blanks);
							elseif($codeblock && $blanks > 0)
								$sstr .= str_repeat('<br />', $blanks);
							if($nindent == 1)
								$sstr .= parse_common_types($l);
							else {
								$sstr .= $l;
								if($nindent == 0)
									$absorbed = true;
							}
						}
						
						$blanks = 0;
					}
					
					$pline++;
				}
			}
		}
		
		fclose($f);
	}
}

function code_category_jumps() {
	global $_categories;
	
	$out = '';
	foreach($_categories as $category) {
		$out .= jump($category).'<br />';
	}
	
	return $out;
}

function add_code_chapters() {
	global $_chapters, $_categories;
	
	foreach($_categories as $category)
		array_push($_chapters, $category);
}

function parse_common_types($str) {
	return preg_replace('/(\W|^)(float|bool|int|string|GLubyte|pixelcache)(\W|$)/', '$1<span class="type">$2</span>$3', $str);
}

function parse_links($str) {
	global $_definitions;
	/*echo '<pre>';
	var_dump($_definitions);
	echo '</pre>';*/
	return preg_replace('/(\W|^)('.implode('|', $_definitions).')(\W|$)/', '$1<a href="#$2">$2</a>$3', $str);
}
