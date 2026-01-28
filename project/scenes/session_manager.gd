extends Node
class_name SessionManager

@export var chess_board: ChessBoard

@export var screen_parent: Node
@export var undo_button: BaseButton
@export var redo_button: BaseButton
@export var restart_button: BaseButton

@export
var _king_can_defuse: bool = true
@export
var _large_explosions: bool = true
@export
var _mine_regeneration: bool = true
@export_range(0,32,1)
var _mine_count: int = 6

var _seed: int = randi()

var _previous_seed: int

@export var king_can_defuse_button: CheckButton
@export var large_explosions_button: CheckButton
@export var mine_regeneration_button: CheckButton
@export var mine_count_range: Range

@export var seed_lock_toggle: BaseButton
@export var reseed_button: BaseButton
@export var seed_text: LineEdit

var _game: MineChessGame

var _has_ended: bool = false
var _is_running: bool  = false
var _selected_figure: MineChessPosition = null

var _promote_to: MineChessFigure.FigureType = MineChessFigure.QUEEN

signal game_started()
signal game_ended(result: MineChessGame.EndResult, reason: MineChessGame.EndReason)
signal game_reset()

func _ready():
	mine_count_range.value = _mine_count
	king_can_defuse_button.button_pressed = _king_can_defuse
	large_explosions_button.button_pressed = _large_explosions
	mine_regeneration_button.button_pressed = _mine_regeneration
	
	_prepare_game()
	chess_board.connect("tile_selected", _on_tile_selected)
	
	undo_button.disabled = true
	redo_button.disabled = true
	restart_button.disabled = true
	
	undo_button.connect("pressed", _on_undo_button_pressed)
	redo_button.connect("pressed", _on_redo_button_pressed)
	restart_button.connect("pressed", _on_restart_button_pressed)
	
	connect("game_started", _on_game_started)
	connect ("game_reset", _on_game_reset)
	
	king_can_defuse_button.disabled = false
	large_explosions_button.disabled= false
	mine_regeneration_button.disabled= false
	
	king_can_defuse_button.button_pressed = _king_can_defuse
	large_explosions_button.button_pressed = _large_explosions
	mine_regeneration_button.button_pressed = _mine_regeneration
	
	seed_text.text = "{0}".format([_seed])
	king_can_defuse_button.connect("toggled", _on_king_can_defuse_button_toggled)
	large_explosions_button.connect("toggled", _on_large_explosions_button_toggled)
	mine_regeneration_button.connect("toggled", _on_mine_regeneration_button_toggled)
	mine_count_range.connect("value_changed", _on_mine_count_value_changed)
	reseed_button.connect("pressed", _on_reseed_pressed)
	seed_text.connect("text_submitted", _on_seed_text_submitted)
	seed_text.connect("mouse_exited", _on_seed_text_mouse_exited)

func _on_game_started():
	_previous_seed = _seed
	restart_button.disabled = false
	king_can_defuse_button.disabled = true
	large_explosions_button.disabled= true
	mine_regeneration_button.disabled= true
	reseed_button.disabled = true
	seed_text.editable = false
	mine_count_range.editable = false

func _on_game_reset():
	undo_button.disabled = true
	redo_button.disabled = true
	restart_button.disabled = true
	king_can_defuse_button.disabled = false
	large_explosions_button.disabled= false
	mine_regeneration_button.disabled= false
	mine_count_range.editable = true
	reseed_button.disabled = false
	seed_text.editable = true

func _on_tile_selected(position: MineChessPosition, figure: MineChessFigure, movement_possible: bool):
	if (_has_ended):
		return
	
	if (_selected_figure == null):
		if figure != null && figure.color == _game.get_current_player():
			_selected_figure = position
			update_board()
		return
		
	if (_selected_figure == position):
		_selected_figure = null
		update_board()
		return
	
	if !movement_possible:
		if (figure != null && figure.color == _game.get_current_player()):
			_selected_figure = position
		else:
			_selected_figure = null
		update_board()
		return
	
	await make_move(_selected_figure, position)
	_selected_figure = null
	update_board()
	return

func _on_promotion_type_selected(to_promote_to: MineChessFigure.FigureType):
	_promote_to = to_promote_to

func make_move(from: MineChessPosition, to:MineChessPosition)->bool:
	var figure : MineChessFigure = _game.get_figuredata(from)
	if (figure.figure_type == MineChessFigure.PAWN):
		if (to.rank == MineChessPosition.RANK_1 || to.rank == MineChessPosition.RANK_8):
			var promotion_screen: PromotionScreen = PromotionScreen.create()
			promotion_screen.connect("selected", _on_promotion_type_selected)
			screen_parent.add_child(promotion_screen)
			await promotion_screen.selected
	
	if (!_game.try_move_figure(from, to, _promote_to)):
		return false
	if(!_is_running):
		emit_signal("game_started")
		_is_running = true
	undo_button.disabled = false
	redo_button.disabled = true
	return true
	
func update_board():
	chess_board.clear();
	for figure: MineChessFigure in _game.get_figures():
		chess_board.set_figure(figure)
		if(_selected_figure == null && figure.figure_type == MineChessFigure.KING):
			if(_game.is_in_check(figure.position, figure.color)):
				chess_board.set_in_check(figure.position)
	if (_selected_figure != null):
		chess_board.select_figure(_selected_figure)
		for target_pos in _game.get_legal_moves(_selected_figure):
			chess_board.show_movement_indicator(target_pos)

func _prepare_game():
	if (!_is_running):
		_game = MineChessGame.create(_mine_count, _king_can_defuse, _large_explosions, _mine_regeneration, _seed)
		_game.connect("explosion", _on_explosion)
		_game.connect("game_ended", _on_game_ended)
		update_board()

func _on_explosion(position: MineChessPosition):
	chess_board.spawn_explosion(position, _game.get_large_explosions())

func _on_game_ended(result: MineChessGame.EndResult, reason: MineChessGame.EndReason):
	restart_button.disabled = true;
	undo_button.disabled = true;
	_has_ended = true
	print("game ended")
	emit_signal("game_ended", result, reason)
	var reasonstr: String
	var resultstr: String
	var delay: float = 0;

	match(reason):
		MineChessGame.CHECKMATE:
			reasonstr = "checkmate"
		MineChessGame.STALEMATE:
			reasonstr = "stalemate"
		MineChessGame.INSUFFICIENT_MATERIAL:
			reasonstr = "insufficient material"
		MineChessGame.EXPLOSION:
			reasonstr="explosion"
			delay = 0.5
		MineChessGame.FAILED_DEFENSE:
			reasonstr="failed defense"
	match(result):
		MineChessGame.BLACK_WON:
			resultstr= "Black won"
		MineChessGame.WHITE_WON:
			resultstr= "White won"
		MineChessGame.DRAW:
			resultstr= "Draw"
	await get_tree().create_timer(delay).timeout
	var info_screen = InfoScreen.create("{0} by {1}!".format([resultstr, reasonstr]), "Restart")
	screen_parent.add_child(info_screen)
	info_screen.connect("closed", restart)

func is_running()->bool:
	return _is_running

func has_ended()->bool:
	return _has_ended

func restart():
	if(!_is_running):
		return
	_has_ended = false
	_is_running = false
	if (!seed_lock_toggle.button_pressed) && (_seed == _previous_seed):
		_seed = randi()
		seed_text.text = "{0}".format([_seed])
	_prepare_game()
	emit_signal("game_reset")

func _on_restart_button_pressed():
	if (_is_running):
		var confirmation_screen: ConfirmationScreen = ConfirmationScreen.create("Do you really want to restart?", "yes", "no")
		screen_parent.add_child(confirmation_screen)
		confirmation_screen.connect("confirmed", restart)

func _on_mine_regeneration_button_toggled(toggled_on):
	if (_is_running):
		return
	_mine_regeneration = toggled_on
	_prepare_game()

func _on_large_explosions_button_toggled(toggled_on):
	if (_is_running):
		return
	_large_explosions = toggled_on
	_prepare_game()

func _on_king_can_defuse_button_toggled(toggled_on):
	if (_is_running):
		return
	_king_can_defuse = toggled_on
	_prepare_game()

func _on_undo_button_pressed():
	_game.undo()
	update_board()
	undo_button.disabled = !_game.can_undo()
	redo_button.disabled = !_game.can_redo()

func _on_redo_button_pressed():
	_game.redo()
	update_board()
	undo_button.disabled = !_game.can_undo()
	redo_button.disabled = !_game.can_redo()

func _on_mine_count_value_changed(value):
	_mine_count = value
	_prepare_game()

func _on_reseed_pressed():
	_seed = randi()
	seed_text.text = "{0}".format([_seed])
	_prepare_game()

func _on_seed_text_submitted(value: String):
	seed_text.release_focus()
	_seed = int(value)
	seed_text.text = "{0}".format([_seed])
	_prepare_game()
	print(value)

func _on_seed_text_mouse_exited():
	seed_text.release_focus()
	seed_text.text = "{0}".format([_seed])
