extends Control
class_name ChessBoard

@export var color_theme: ColorTheme

@export var iconset: Figureset

@export var grid: GridContainer

signal tile_selected(position: MineChessPosition, figure: MineChessFigure, movement_possible: bool)

signal selection_cancelled()

var _selected_tile: ChessTile

var _tiles = {}

func _enter_tree():
	for rank in 8:
		for file in 8:
			_add_tile(file, 7-rank)

func _add_tile(file: int, rank: int):
	var tile_position = MineChessPosition.create(file, rank)
	var new_tile: ChessTile = ChessTile.create(self, tile_position)
	grid.add_child(new_tile)
	_tiles[tile_position.index] = new_tile
	new_tile.connect("tile_selected", _on_tile_selected)

func _on_tile_selected(tile: ChessTile):
	emit_signal("tile_selected", tile.get_chess_position(), tile.figure, tile.movement_possible)

func _on_selection_cancelled():
	emit_signal("selection_cancelled")

func select_figure(chess_position: MineChessPosition):
	if _selected_tile != null:
		_selected_tile.unselect()
	if chess_position == null:
		_selected_tile = null
	else:
		_selected_tile = _tiles[chess_position.index]
		_selected_tile.select()
	
func show_movement_indicator(chess_position: MineChessPosition):
	_tiles[chess_position.index].show_movement_indicator()

func clear():
	for tile_index in _tiles:
		var tile: ChessTile = _tiles[tile_index]
		tile.clear()

func set_figure(figure: MineChessFigure):
	var tile: ChessTile = _tiles[figure.position.index]
	tile.set_figure(figure)

func set_in_check(chess_position: MineChessPosition):
	var tile: ChessTile = _tiles[chess_position.index]
	tile.set_in_check()

func spawn_explosion(chess_position: MineChessPosition, is_large: bool):
	var tile: ChessTile = _tiles[chess_position.index]
	var explosion: Explosion = Explosion.create(chess_position, tile, is_large);
	tile.add_child(explosion)
