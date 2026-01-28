extends VBoxContainer
class_name SmartVBoxContainer

@export
var inline_alignment: AlignmentMode = ALIGNMENT_CENTER
@export
var orthogonal_alignment: AlignmentMode = ALIGNMENT_CENTER

@export
var inverted: bool = false

func _process(_delta):
	var window_size: Vector2 = get_viewport().get_visible_rect().size;
	if (window_size.x >= window_size.y):
		alignment = inline_alignment
	else:
		alignment = orthogonal_alignment
