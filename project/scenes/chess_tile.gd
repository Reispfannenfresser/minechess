extends Control
class_name ChessTile

const self_scene: PackedScene = preload("res://scenes/chess_tile.tscn")

@export
var board_texture: TextureRect

@export
var figure_texture: TextureRect

@export
var indicator_texture: TextureRect 

@export
var mine_count_texture: TextureRect

var figure: MineChessFigure = null

var movement_possible: bool = false

var _board: ChessBoard

var _chess_position: MineChessPosition

var _is_in_check = false

signal tile_selected(tile: ChessTile)

static func create(board: ChessBoard, chess_position: MineChessPosition)-> ChessTile:
	var new_tile = self_scene.instantiate()
	new_tile._chess_position = chess_position;
	new_tile._board = board;
	if ((chess_position.file + chess_position.rank) % 2 == 0):
		new_tile.board_texture.self_modulate = board.color_theme.black_board_color
	else:
		new_tile.board_texture.self_modulate = board.color_theme.white_board_color
	new_tile.indicator_texture.self_modulate = board.color_theme.indicator_color
	new_tile.mine_count_texture.self_modulate = board.color_theme.indicator_color
	return new_tile

func select():
	indicator_texture.texture = _board.iconset.select

func unselect():
	indicator_texture.texture = null

func show_movement_indicator():
	movement_possible = true
	if figure_texture.texture == null:
		indicator_texture.texture = _board.iconset.move
	else:
		indicator_texture.texture = _board.iconset.capture

func hide_movement_indicator():
	movement_possible = false
	indicator_texture.texture = _board.iconset.move

func get_chess_position()-> MineChessPosition:
	return _chess_position

func set_figure(p_figure: MineChessFigure):
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
		MineChessFigure.ROOK:
			figure_texture.texture = _board.iconset.rook
		MineChessFigure.KNIGHT:
			figure_texture.texture = _board.iconset.knight
		MineChessFigure.BISHOP:
			figure_texture.texture = _board.iconset.bishop
		MineChessFigure.QUEEN:
			figure_texture.texture = _board.iconset.queen
		MineChessFigure.KING:
			figure_texture.texture = _board.iconset.king
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

func set_in_check():
	_is_in_check = true
	indicator_texture.texture = _board.iconset.capture

func clear():
	figure = null
	movement_possible = false
	figure_texture.texture = null
	indicator_texture.texture = null
	mine_count_texture.texture = null
	_is_in_check = false

func _on_button_pressed():
	emit_signal("tile_selected", self)
