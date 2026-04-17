extends Screen
class_name InfoScreen

@export
var _hint: Label
@export
var close_button: Button

static var self_scene: PackedScene = preload("res://scenes/screens/info_screen.tscn")

static func create(info: String, close_text: String)-> InfoScreen:
	var new_info_screen: InfoScreen = self_scene.instantiate()
	new_info_screen._hint.text = info
	new_info_screen.close_button.text = close_text
	return new_info_screen

func _on_close_button_pressed()-> void:
	close()
