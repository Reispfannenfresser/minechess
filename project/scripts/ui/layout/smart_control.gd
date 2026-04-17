class_name SmartControl
extends Control

@export
var _landscape_size_flags_horizontal: SizeFlags = size_flags_horizontal as SizeFlags
@export
var _landscape_size_flags_vertical: SizeFlags = size_flags_vertical as SizeFlags
@export
var _portrait_size_flags_horizontal: SizeFlags = size_flags_horizontal as SizeFlags
@export
var _portrait_size_flags_vertical: SizeFlags = size_flags_vertical as SizeFlags

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
	size_flags_horizontal = _landscape_size_flags_horizontal
	size_flags_vertical = _landscape_size_flags_vertical
	
func _switch_to_portrait()-> void:
	size_flags_horizontal = _portrait_size_flags_horizontal
	size_flags_vertical = _portrait_size_flags_vertical
