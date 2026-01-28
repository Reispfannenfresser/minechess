extends Panel
class_name ConfirmationScreen

signal confirmed()
signal cancelled()

@export
var _hint: Label
@export
var _confirm: Button
@export
var _cancel: Button

static var self_scene: PackedScene = preload("res://scenes/confirmation_screen.tscn")

static func create(hint: String, confirm: String, cancel: String)-> ConfirmationScreen:
	var new_confirmation_screen: ConfirmationScreen = self_scene.instantiate()
	new_confirmation_screen._hint.text = hint
	new_confirmation_screen._confirm.text = confirm
	new_confirmation_screen._cancel.text = cancel
	return new_confirmation_screen

func _on_confirm_button_pressed():
	emit_signal("confirmed")
	queue_free()

func _on_cancel_button_pressed():
	emit_signal("cancelled")
	queue_free()
