extends Control
class_name ChessBoard

@export var color_theme: ColorTheme

@export var iconset: Figureset

@export var grid: GridContainer

@export var screen_parent: Node

signal game_started()
signal game_ended()
signal game_prepared()
signal restart_requested()
signal figures_updated()
signal screen_shown()
signal screen_closed()

var _tiles: Dictionary = {}

var _running: bool = false

var _game: MineChessGame = null

var _selected_tile: ChessTile

var _current_screen: Screen = null

var _promotion_type: MineChessFigure.FigureType = MineChessFigure.QUEEN

func _enter_tree()-> void:
	_create_tiles()

func _create_tiles()-> void:
	for rank: int in 8:
		for file: int in 8:
			var tile_position: MineChessPosition = MineChessPosition.create(file, 7-rank)
			var new_tile: ChessTile = ChessTile.create(self, tile_position)
			new_tile.selection_clear_requested.connect(_on_tile_selection_clear_requested)
			new_tile.selection_requested.connect(_on_tile_selection_requested)
			new_tile.movement_requested.connect(_on_tile_movement_requested)
			grid.add_child(new_tile)
			_tiles[tile_position.index] = new_tile

func _try_move(from: MineChessPosition, to: MineChessPosition)->bool:
	if(!_tiles[to.index].is_movement_target()):
		return false
	
	var figure: MineChessFigure = _game.get_figuredata(from)
	if(figure != null && figure.figure_type == MineChessFigure.PAWN && (to.rank == MineChessPosition.RANK_1 || to.rank == MineChessPosition.RANK_8)):
		var promotion_screen: PromotionScreen = PromotionScreen.create()
		promotion_screen.selected.connect(_on_promotion_type_selected)
		set_current_screen(promotion_screen)
		await promotion_screen.selected
	if (_game.try_move_figure(from, to, _promotion_type)):
		if(!_running):
			_running = true
			game_started.emit()
		update_figures()
		_selected_tile = null
		return true
	_clear_selection()
	return false

func _on_promotion_type_selected(promotion_type: MineChessFigure.FigureType)-> void:
	_promotion_type = promotion_type

func _on_tile_movement_requested(to: MineChessPosition)-> void:
	print("Movement requested to {0}, {1}".format([to.file, to.rank]))
	if(_selected_tile == null):
		return
	var from: MineChessPosition = _selected_tile.get_chess_position()
	_try_move(from, to)

func _on_tile_selection_clear_requested()-> void:
	_clear_selection()

func _on_tile_selection_requested(tile: ChessTile)-> void:
	print("Selection requested at {0}, {1}".format([tile.get_chess_position().file, tile.get_chess_position().rank]))
	_clear_selection()
	var figure_data: MineChessFigure = _game.get_figuredata(tile._chess_position)
	if(figure_data != null && figure_data.color == _game.get_current_player()):
		tile.select()
		_selected_tile = tile
		for move_position: MineChessPosition in _game.get_legal_moves(tile.get_chess_position()):
			_tiles[move_position.index].show_movement_indicator()

func _clear_selection()-> void:
	if(_selected_tile == null):
		return
	for move_position: MineChessPosition in _game.get_legal_moves(_selected_tile.get_chess_position()):
		_tiles[move_position.index].hide_movement_indicator()
	_selected_tile.unselect()
	_selected_tile = null

func prepare(mine_count: int, king_can_defuse: bool, large_explosions: bool, mine_regeneration: bool, game_seed: int)-> void:
	if (_game != null):
		_game.explosion.disconnect(_on_explosion)
		_game.game_ended.disconnect(_on_game_ended)

	_game = MineChessGame.create(mine_count, king_can_defuse, large_explosions, mine_regeneration, game_seed)

	_game.explosion.connect(_on_explosion)
	_game.game_ended.connect(_on_game_ended)

	_running = false
	update_figures()
	game_prepared.emit()

func update_figures()-> void:
	_clear();
	var current_player: MineChessFigure.Color = _game.get_current_player()
	for figure: MineChessFigure in _game.get_figures():
		var tile: ChessTile = _tiles[figure.position.index]
		tile.set_figure(figure)
		if(figure.color == current_player):
			tile.allow_movement()
		if(figure.figure_type == MineChessFigure.KING):
			if(_game.is_in_check(figure.position, figure.color)):
				tile.set_in_check()
	figures_updated.emit()

func _clear()-> void:
	_clear_selection()
	for tile_index: int in _tiles:
		var tile: ChessTile = _tiles[tile_index]
		tile.clear()

func _on_explosion(chess_position: MineChessPosition)-> void:
	var tile: ChessTile = _tiles[chess_position.index]
	var explosion: Explosion = Explosion.create(chess_position, tile, _game.get_large_explosions());
	tile.add_child(explosion)

func _on_game_ended(result: MineChessGame.EndResult, reason: MineChessGame.EndReason)-> void:
	await get_tree().create_timer(0.5).timeout

	_running = false
	game_ended.emit()

	var info_screen: InfoScreen = InfoScreen.create(compose_end_message(result, reason), "Restart")
	info_screen.connect("closed", _on_end_screen_closed)
	set_current_screen(info_screen)

func _on_end_screen_closed(_screen: Screen)-> void:
	restart_requested.emit()

func compose_end_message(result: MineChessGame.EndResult, reason: MineChessGame.EndReason)->String:
	var reasonstr: String
	var resultstr: String

	match(result):
		MineChessGame.BLACK_WON:
			resultstr = "Black won"
		MineChessGame.WHITE_WON:
			resultstr = "White won"
		MineChessGame.DRAW:
			resultstr = "Draw"

	match(reason):
		MineChessGame.CHECKMATE:
			reasonstr = "checkmate"
		MineChessGame.STALEMATE:
			reasonstr = "stalemate"
		MineChessGame.INSUFFICIENT_MATERIAL:
			reasonstr = "insufficient material"
		MineChessGame.EXPLOSION:
			reasonstr = "explosion"
		MineChessGame.FAILED_DEFENSE:
			reasonstr = "failed defense"

	return "{0} by {1}!".format([resultstr, reasonstr])

func is_running()->bool:
	return _running

func has_screen()->bool:
	return _current_screen != null

func is_controllable()->bool:
	return is_running() && !has_screen()

func set_current_screen(screen: Screen)-> void:
	if(_current_screen != null):
		_current_screen.cancel()
	_current_screen = screen
	_current_screen.closed.connect(_on_current_screen_closed)
	screen_parent.add_child(screen)
	screen_shown.emit()

func _on_current_screen_closed(_screen: Screen)-> void:
	_current_screen = null
	screen_closed.emit()

func can_undo()-> bool:
	if(_game == null):
		return false
	return _game.can_undo()

func can_redo()-> bool:
	if(_game == null):
		return false
	return _game.can_redo()

func try_undo()-> bool:
	if(_game != null && _game.can_undo()):
		_game.undo()
		update_figures()
		return true
	return false

func try_redo()-> bool:
	if(_game != null && _game.can_redo()):
		_game.redo()
		update_figures()
		return true
	return false

func try_restart()-> void:
	var restart_confirmation_screen: ConfirmationScreen = ConfirmationScreen.create("Do you really want to restart?", "Yes", "No")
	restart_confirmation_screen.confirmed.connect(_on_restart_confirmed)
	set_current_screen(restart_confirmation_screen)

func _on_restart_confirmed()-> void:
	restart_requested.emit()
