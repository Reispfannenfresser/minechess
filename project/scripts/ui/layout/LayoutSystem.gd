extends Node

var landscape: bool = false:
	get:
		return landscape

signal switched_to_landscape()
signal switched_to_portrait()

func _process(_delta: float)-> void:
	var window_size: Vector2 = get_viewport().get_visible_rect().size;
	var desired_landscape: bool = window_size.x >= window_size.y
	
	if (landscape == desired_landscape):
		return
	
	if(desired_landscape):
		landscape = true
		switched_to_landscape.emit()
	else:
		landscape = false
		switched_to_portrait.emit()
