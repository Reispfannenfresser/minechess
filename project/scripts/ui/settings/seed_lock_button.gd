extends Button
class_name SeedLockButton

@export var _session_manager: SessionManager
@export var _texture_rect: TextureRect

@export var on_texture: Texture2D
@export var off_texture: Texture2D

func _ready()-> void:
	set_pressed_no_signal(_session_manager.is_seed_locked())
	_update_texture()
	_session_manager.seed_locked_changed.connect(_on_seed_locked_changed)
	toggled.connect(_on_toggled)

func _on_seed_locked_changed(new_value: bool)-> void:
	set_pressed_no_signal(new_value)
	_update_texture()

func _on_toggled(toggled_on: float)-> void:
	if(_session_manager.try_set_seed_locked(toggled_on)):
		_update_texture()
	else:
		set_pressed_no_signal(_session_manager.is_seed_locked())

func _update_texture()-> void:
	if button_pressed:
		_texture_rect.texture = on_texture
	else:
		_texture_rect.texture = off_texture
