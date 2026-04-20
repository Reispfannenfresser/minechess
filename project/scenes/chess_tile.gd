extends Control
class_name ChessTile

const self_scene: PackedScene = preload("res://scenes/chess_tile.tscn")

@export
var board_texture: TextureRect

@export
var figure_texture: TextureRect
@export
var shadow_texture: TextureRect

@export
var indicator_texture: TextureRect

@export
var mine_count_texture: TextureRect

var figure: MineChessFigure = null

var _is_movement_target: bool = false
var _can_move: bool = false
var _is_selected: bool = false

var _board: ChessBoard

var _chess_position: MineChessPosition

var _is_in_check: bool = false

var _is_holding: bool = false
var _was_selected: bool = false
var _is_dragging: bool = false
var _hold_duration: float = 0

signal selection_requested(tile: ChessTile)
signal selection_clear_requested
signal movement_requested(position: MineChessPosition)

static func create(board: ChessBoard, chess_position: MineChessPosition)-> ChessTile:
	var new_tile: ChessTile = self_scene.instantiate()
	new_tile._chess_position = chess_position;
	new_tile._board = board;
	if ((chess_position.file + chess_position.rank) % 2 == 0):
		new_tile.board_texture.self_modulate = board.color_theme.black_board_color
	else:
		new_tile.board_texture.self_modulate = board.color_theme.white_board_color
	new_tile.indicator_texture.self_modulate = board.color_theme.indicator_color
	new_tile.shadow_texture.self_modulate = board.color_theme.indicator_color
	new_tile.shadow_texture.visible = false
	new_tile.mine_count_texture.self_modulate = board.color_theme.indicator_color
	return new_tile

func select()-> void:
	indicator_texture.texture = _board.iconset.select
	shadow_texture.visible = true
	_is_selected = true;

func unselect()-> void:
	if(_is_in_check):
		indicator_texture.texture = _board.iconset.capture
	else:
		indicator_texture.texture = null
	shadow_texture.visible = false
	_is_selected = false

func show_movement_indicator()-> void:
	_is_movement_target = true
	if figure_texture.texture == null:
		indicator_texture.texture = _board.iconset.move
	else:
		indicator_texture.texture = _board.iconset.capture

func hide_movement_indicator()-> void:
	_is_movement_target = false
	indicator_texture.texture = null

func is_movement_target()-> bool:
	return _is_movement_target

func allow_movement()-> void:
	_can_move = true

func disallow_movement()-> void:
	_can_move = false

func can_move()-> bool:
	return _can_move

func get_chess_position()-> MineChessPosition:
	return _chess_position

func set_figure(p_figure: MineChessFigure)-> void:
	figure = p_figure
	if(figure == null):
		figure_texture.texture = null
		return

	if (figure.color == MineChessFigure.WHITE):
		figure_texture.self_modulate = _board.color_theme.white_figure_color
	else:
		figure_texture.self_modulate = _board.color_theme.black_figure_color

	match figure.figure_type:
		MineChessFigure.PAWN:
			figure_texture.texture = _board.iconset.pawn
			shadow_texture.texture = _board.iconset.pawn_shadow
		MineChessFigure.ROOK:
			figure_texture.texture = _board.iconset.rook
			shadow_texture.texture = _board.iconset.rook_shadow
		MineChessFigure.KNIGHT:
			figure_texture.texture = _board.iconset.knight
			shadow_texture.texture = _board.iconset.knight_shadow
		MineChessFigure.BISHOP:
			figure_texture.texture = _board.iconset.bishop
			shadow_texture.texture = _board.iconset.bishop_shadow
		MineChessFigure.QUEEN:
			figure_texture.texture = _board.iconset.queen
			shadow_texture.texture = _board.iconset.queen_shadow
		MineChessFigure.KING:
			figure_texture.texture = _board.iconset.king
			shadow_texture.texture = _board.iconset.king_shadow
	match figure.mine_count:
		0:
			mine_count_texture.texture = null
		1:
			mine_count_texture.texture=_board.iconset.mine_1
		2:
			mine_count_texture.texture=_board.iconset.mine_2
		3:
			mine_count_texture.texture=_board.iconset.mine_3
		4:
			mine_count_texture.texture=_board.iconset.mine_4
		5:
			mine_count_texture.texture=_board.iconset.mine_5
		6:
			mine_count_texture.texture=_board.iconset.mine_6
		7:
			mine_count_texture.texture=_board.iconset.mine_7
		8:
			mine_count_texture.texture=_board.iconset.mine_8

func set_in_check()-> void:
	_is_in_check = true
	indicator_texture.texture = _board.iconset.capture

func clear()-> void:
	figure = null
	_is_selected = false
	_was_selected = false
	_can_move = false
	_is_movement_target = false
	figure_texture.texture = null
	shadow_texture.texture = null
	indicator_texture.texture = null
	mine_count_texture.texture = null
	_is_in_check = false
	_is_holding = false
	_hold_duration = 0
	_is_dragging = false

func _on_button_down()-> void:
	_hold_duration = 0
	_is_holding = true
	_was_selected = _is_selected

func _on_button_up()-> void:
	_hold_duration = 0
	_is_holding = false
	
	if(_is_dragging):
		_stop_dragging()
		return
	
	if(_is_movement_target):
		movement_requested.emit(_chess_position)
		return
	
	if(!_can_move):
		selection_clear_requested.emit()
		return

	if(_was_selected):
		selection_clear_requested.emit()
	else:
		selection_requested.emit(self)


func _process(delta: float)-> void:
	if(_is_holding):
		if(_can_move && !_is_dragging && _hold_duration >= 0.1):
			_start_dragging()
		_hold_duration += delta
	if(_is_dragging):
		figure_texture.position = get_global_mouse_position() - (figure_texture.size * 0.5)

func _start_dragging()-> void:
	_is_dragging = true
	figure_texture.top_level = true
	selection_requested.emit(self)

func _stop_dragging()-> void:
	var drop_position: Vector2i = _get_drop_position()
	
	_is_dragging = false
	figure_texture.top_level = false
	figure_texture.position = Vector2.ZERO
	
	if(drop_position.x == _chess_position.file && drop_position.y == _chess_position.rank):
		if(_was_selected):
			selection_clear_requested.emit()
		else:
			selection_requested.emit(self)
		return
	
	if(drop_position.x >= 0 && drop_position.x <= 7 && drop_position.y >= 0 && drop_position.y <= 7):
		movement_requested.emit(MineChessPosition.create(drop_position.x, drop_position.y))
	else:
		selection_clear_requested.emit()

func _get_drop_position()-> Vector2i:
	var offset: Vector2 = figure_texture.global_position - board_texture.global_position
	offset = round(offset / figure_texture.size)
	offset.y *= -1
	return Vector2i(_chess_position.file + int(offset.x), _chess_position.rank + int(offset.y))
