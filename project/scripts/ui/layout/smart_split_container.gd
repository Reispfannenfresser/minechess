extends SplitContainer
class_name SmartSplitter

@export
var split_ratio: float = 0.66666

func _ready()-> void:
	LayoutSystem.connect("switched_to_landscape", _on_layout_system_switched_to_landscape)
	LayoutSystem.connect("switched_to_portrait", _on_layout_system_switched_to_portrait)
	
	if(LayoutSystem.landscape):
		_switch_to_landscape()
	else:
		_switch_to_portrait()

func _on_layout_system_switched_to_landscape()-> void:
	_switch_to_landscape()

func _on_layout_system_switched_to_portrait()-> void:
	_switch_to_portrait()

func _switch_to_landscape()-> void:
	vertical = false

func _switch_to_portrait()-> void:
	vertical = true

func _process(_delta: float)-> void:
	var window_size: Vector2 = get_viewport().get_visible_rect().size;
	var large_edge: float = max(window_size.x, window_size.y)
	var small_edge: float = min(window_size.x, window_size.y)
	
	var board_size: float = min(small_edge, large_edge * split_ratio)
	
	split_offset = int(board_size - 0.5 * large_edge) 
