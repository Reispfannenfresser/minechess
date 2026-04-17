extends LineEdit
class_name SeedLineEdit

@export var _session_manager: SessionManager

func _ready()-> void:
	text = "{0}".format([_session_manager.get_game_seed()])
	_session_manager.game_seed_changed.connect(_on_game_seed_changed)
	_session_manager.session_state_changed.connect(_on_session_state_changed)
	text_submitted.connect(_on_text_submitted)
	mouse_exited.connect(_on_mouse_exited)

func _on_game_seed_changed(new_value: int)-> void:
	text = "{0}".format([new_value])

func _on_session_state_changed(new_value: SessionManager.SessionState)-> void:
	editable = new_value == SessionManager.SessionState.PREPARING

func _on_text_submitted(new_value: String)-> void:
	_session_manager.try_set_game_seed(int(new_value))
	release_focus()

func _on_mouse_exited()-> void:
	_session_manager.try_set_game_seed(int(text))
	release_focus()
