extends Button
class_name RedoButton

@export var _chess_board: ChessBoard = null

func _ready()-> void:
	update_disabled_state()
	_chess_board.game_started.connect(update_disabled_state)
	_chess_board.game_ended.connect(update_disabled_state)
	_chess_board.figures_updated.connect(update_disabled_state)
	_chess_board.game_prepared.connect(update_disabled_state)
	_chess_board.screen_shown.connect(update_disabled_state)
	_chess_board.screen_closed.connect(update_disabled_state)
	pressed.connect(_on_pressed)

func update_disabled_state()-> void:
	disabled = !_chess_board.is_controllable() || !_chess_board.can_redo()

func _on_pressed()-> void:
	_chess_board.try_redo()
