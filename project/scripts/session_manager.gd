extends Node
class_name SessionManager

@export var chess_board: ChessBoard

@export_group("initial game settings")
@export_range(0,32,1) var _mine_count: int = 6
@export var _king_can_defuse: bool = true
@export var _large_explosions: bool = true
@export var _mine_regeneration: bool = true
var _game_seed: int = randi()
@export var _seed_locked: bool = false


enum SessionState {PREPARING, STARTED, ENDED}

signal session_state_changed(new_value: SessionState)

var _session_state: SessionState = SessionState.PREPARING:
	get:
		return _session_state
	set(value):
		_session_state = value
		session_state_changed.emit(value)


signal mine_count_changed(new_value: int)
signal king_can_defuse_changed(new_value: bool)
signal large_explosions_changed(new_value: bool)
signal mine_regeneration_changed(new_value: bool)
signal game_seed_changed(new_value: bool)
signal seed_locked_changed(new_value: bool)


func _ready()-> void:
	_prepare_board()
	chess_board.game_started.connect(_on_game_started)
	chess_board.game_ended.connect(_on_game_ended)
	chess_board.restart_requested.connect(_on_restart_requested)

func _prepare_board()-> void:
	chess_board.prepare(_mine_count, _king_can_defuse, _large_explosions,_mine_regeneration,_game_seed)

func _on_game_started()-> void:
	if(_session_state == SessionState.PREPARING):
		_session_state = SessionState.STARTED

func _on_game_ended()-> void:
	if(_session_state == SessionState.STARTED):
		_session_state = SessionState.ENDED

func _on_restart_requested()-> void:
	if(_session_state != SessionState.PREPARING):
		_session_state = SessionState.PREPARING
		if(!_seed_locked):
			try_randomize_seed()
		_prepare_board()

func get_session_state()->SessionState:
	return _session_state

func get_mine_count()->int:
	return _mine_count

func get_king_can_defuse()->bool:
	return _king_can_defuse

func get_large_explosions()->bool:
	return _large_explosions

func get_mine_regeneration()->bool:
	return _mine_regeneration

func get_game_seed()->int:
	return _game_seed

func is_seed_locked()->bool:
	return _seed_locked

func try_set_mine_count(value: int)->bool:
	if(_session_state == SessionState.PREPARING):
		_mine_count = value
		_prepare_board()
		mine_count_changed.emit(value)
		return true
	return false

func try_set_king_can_defuse(value: bool)->bool:
	if(_session_state == SessionState.PREPARING):
		_king_can_defuse = value
		_prepare_board()
		king_can_defuse_changed.emit(value)
		return true
	return false

func try_set_large_explosions(value: bool)->bool:
	if(_session_state == SessionState.PREPARING):
		_large_explosions = value
		_prepare_board()
		large_explosions_changed.emit(value)
		return true
	return false

func try_set_mine_regeneration(value: bool)->bool:
	if(_session_state == SessionState.PREPARING):
		_mine_regeneration = value
		_prepare_board()
		mine_regeneration_changed.emit(value)
		return true
	return false

func try_set_game_seed(value: int)->bool:
	if(_session_state == SessionState.PREPARING):
		_game_seed = value
		_prepare_board()
		game_seed_changed.emit(value)
		return true
	return false

func try_randomize_seed()->bool:
	if(_session_state == SessionState.PREPARING):
		_game_seed = randi()
		_prepare_board()
		game_seed_changed.emit(_game_seed)
		return true
	return false

func try_set_seed_locked(value: bool)->bool:
	_seed_locked = value
	seed_locked_changed.emit(value)
	return true
