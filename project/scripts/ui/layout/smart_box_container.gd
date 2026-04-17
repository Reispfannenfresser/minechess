extends BoxContainer
class_name SmartBoxContainer

@export
var _landscape_alignment: AlignmentMode = ALIGNMENT_CENTER
@export
var _landscape_vertical: bool = false

@export
var _portrait_alignment: AlignmentMode = ALIGNMENT_CENTER
@export
var _portrait_vertical: bool = true

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
	vertical = _landscape_vertical
	alignment = _landscape_alignment
	
func _switch_to_portrait()-> void:
	vertical = _portrait_vertical
	alignment = _portrait_alignment
