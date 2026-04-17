extends Button
class_name RandomSeedButton

@export var _session_manager: SessionManager

func _ready() -> void:
	_session_manager.session_state_changed.connect(_on_session_state_changed)
	pressed.connect(_on_pressed)
	
func _on_session_state_changed(new_value: SessionManager.SessionState)-> void:
	disabled = new_value != SessionManager.SessionState.PREPARING
	
func _on_pressed()-> void:
	_session_manager.try_randomize_seed()
