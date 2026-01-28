extends SplitContainer
class_name SmartSplitter

@export var inverted = false

func _process(_delta):
	var container_size: Vector2 = size;
	vertical = (container_size.y > container_size.x) == !inverted
	
	var max_board_size = max(container_size.x, container_size.y)
	var min_board_size = min(container_size.x, container_size.y)
	
	var board_size = min(min_board_size, max_board_size * 0.6666)
	var desired_setting_space = board_size * 0.5
	var free_space = max_board_size - (board_size + desired_setting_space)
	
	split_offset = max_board_size / -2 + board_size + free_space / 2
