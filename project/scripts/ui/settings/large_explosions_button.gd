extends Button
class_name LargeExplosionsButton

@export var _session_manager: SessionManager

func _ready()-> void:
	set_pressed_no_signal(_session_manager.get_large_explosions())
	_session_manager.session_state_changed.connect(_on_session_state_changed)
	_session_manager.large_explosions_changed.connect(_on_large_explosions_changed)
	toggled.connect(_on_toggled)

func _on_session_state_changed(new_state: SessionManager.SessionState)-> void:
	disabled = new_state != SessionManager.SessionState.PREPARING

func _on_large_explosions_changed(new_value: bool)-> void:
	set_pressed_no_signal(new_value)

func _on_toggled(toggled_on: float)-> void:
	if(!_session_manager.try_set_large_explosions(toggled_on)):
		set_pressed_no_signal(_session_manager.get_large_explosions())
