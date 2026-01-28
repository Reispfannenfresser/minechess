extends BoxContainer
class_name SmartBoxContainer

@export
var inverted: bool = false

func _process(_delta):
	var window_size: Vector2 = get_viewport().get_visible_rect().size;
	vertical = window_size.x >= window_size.y != inverted
