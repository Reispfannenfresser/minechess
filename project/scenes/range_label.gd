extends Label
class_name RangeLabel

@export var _range: Range

func _ready():
	_range.connect("value_changed", _on_value_changed)
	_range.connect("theme_changed", _on_theme_changed)
	text = "{0}".format([int(_range.value)])

func _on_value_changed(value: int):
	text = "{0}".format([value])

func _on_theme_changed():
	print("Theme changed")
	theme = _range.theme
