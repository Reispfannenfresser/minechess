extends AspectRatioContainer
class_name SmartAspectRatioContainer

@export
var inline_alignment: AlignmentMode = ALIGNMENT_CENTER
@export
var orthogonal_alignment: AlignmentMode = ALIGNMENT_CENTER

@export
var inverted: bool = false

var inline_ratio: float = ratio

func _process(_delta):
	var window_size: Vector2 = get_viewport().get_visible_rect().size;
	if (window_size.x >= window_size.y != inverted):
		alignment_horizontal = inline_alignment
		alignment_vertical = orthogonal_alignment
		ratio = inline_ratio
	else:
		alignment_vertical = inline_alignment
		alignment_horizontal = orthogonal_alignment
		ratio = 1/inline_ratio
