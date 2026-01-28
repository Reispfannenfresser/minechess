extends CPUParticles2D

class_name Explosion

const self_scene: PackedScene = preload("res://scenes/explosion.tscn")

var time: float = 2

static func create(chess_position: MineChessPosition, tile: ChessTile, is_large: bool)-> Explosion:
	var explosion: Explosion = self_scene.instantiate()
	
	var min_corner: Vector2 = tile.size / 4
	var max_corner: Vector2 = min_corner * 3
	
	if (is_large):
		if (chess_position.file != MineChessPosition.FILE_A):
			min_corner.x -= tile.size.x
		if (chess_position.file != MineChessPosition.FILE_H):
			max_corner.x += tile.size.x
		if (chess_position.rank != MineChessPosition.RANK_1):
			max_corner.y += tile.size.y
		if (chess_position.rank != MineChessPosition.RANK_8):
			min_corner.y -= tile.size.y	
	
	var explosion_size = max_corner - min_corner
	var volume = explosion_size.x * explosion_size.y
	var tile_volume = tile.size.x * tile.size.y
	explosion.amount *= volume/tile_volume
	explosion.position = min_corner + explosion_size / 2
	explosion.emission_rect_extents = explosion_size / 2
	explosion.emitting = true
	var texture_size = explosion.texture.get_size()
	var tile_size = tile.size
	explosion.scale_amount_min *= tile_size.x / 2 / texture_size.x
	explosion.scale_amount_max = explosion.scale_amount_min
	return explosion

func _process(delta):
	time-=delta;
	if (time < 0):
		queue_free()
