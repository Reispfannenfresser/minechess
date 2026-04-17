extends Screen
class_name ConfirmationScreen

signal confirmed()
signal denied()

@export
var _hint_label: Label
@export
var _confirm_button: Button
@export
var _deny_button: Button

static var self_scene: PackedScene = preload("res://scenes/screens/confirmation_screen.tscn")

static func create(hint_text: String, confirm_text: String, deny_text: String)-> ConfirmationScreen:
	var new_confirmation_screen: ConfirmationScreen = self_scene.instantiate()
	new_confirmation_screen._hint_label.text = hint_text
	new_confirmation_screen._confirm_button.text = confirm_text
	new_confirmation_screen._deny_button.text = deny_text
	return new_confirmation_screen

func _on_confirm_button_pressed() -> void:
	confirmed.emit()
	close()

func _on_deny_button_pressed() -> void:
	denied.emit()
	close()
