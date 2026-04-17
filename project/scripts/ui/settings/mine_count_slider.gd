extends Slider
class_name MineCountSlider

@export var _session_manager: SessionManager

func _ready()-> void:
	set_value_no_signal(_session_manager.get_mine_count())
	_session_manager.session_state_changed.connect(_on_session_state_changed)
	_session_manager.mine_count_changed.connect(_on_mine_count_changed)
	value_changed.connect(_on_value_changed)

func _on_session_state_changed(new_state: SessionManager.SessionState)-> void:
	editable = new_state == SessionManager.SessionState.PREPARING

func _on_mine_count_changed(new_value: int)-> void:
	set_value_no_signal(new_value)

func _on_value_changed(new_value: float)-> void:
	if(!_session_manager.try_set_mine_count(int(new_value))):
		set_value_no_signal(_session_manager.get_mine_count())
