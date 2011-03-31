<?php
class Chapter {
	var $name;
	var $link;
	var $sections = array();
	
	function __construct($name) {
		$this->name = $name;
		$this->link = strtolower($name);
	}
	
	function add_section($name, $content = null) {
		if(is_string($content))
			array_push($this->sections, new Section($name, $content));
		elseif(is_object($name))
			array_push($this->sections, $name);
	}
	
	function add_html($str) {
		array_push($this->sections, $str);
	}
	
	function draw() {
		echo '<br /><a name="'.strtolower($this->name).'"></a><h1>'.$this->name.'</h1>';
		
		foreach($this->sections as $section) {
			if(is_object($section)) {
				if(get_class($section) == 'Section')
					$section->draw();
			}
			elseif(is_string($section)) {
				echo $section;
			}
		}
	}
}

function make_section_link($str) {
	$func_start = strpos($str, '(');
	if($func_start !== false)
		$str = substr($str, 0, $func_start);
	return strtolower($str);
}

class Section {
	var $name;
	var $link;
	var $content;
	var $multi = false;
	
	function __construct($name, $content, $multi = false) {
		$this->name = $name;
		$this->link = (is_null($this->name) ? '' : make_section_link($this->name));
		$this->content = $content;
		$this->multi = $multi;
	}
	
	function draw() {
		echo (!is_null($this->name) ? '<a name="'.$this->link.'"></a>' : '').
		'<div class="section'.($this->multi ? 's' : '').'">'.
		(!is_null($this->name) ? '<a class="definition" href="#'.$this->link.'">'.$this->name.'</a>' : '').
		parse_txt(parse_links($this->content)).'</div>';
	}
}
