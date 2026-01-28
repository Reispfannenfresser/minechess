extends Node2D

func _on_explosion(at: MineChessPosition):
	print("explosion at ({0}, {1})".format([at.file, at.rank]))

func _ready():
	var game: MineChessGame = MineChessGame.create(8, true, true, true, 0)
	
	game.connect("explosion", _on_explosion)
	
	var from: MineChessPosition = MineChessPosition.create(MineChessPosition.FILE_E, MineChessPosition.RANK_2)
	var to: MineChessPosition = MineChessPosition.create(MineChessPosition.FILE_E, MineChessPosition.RANK_4)
	
	if (game.try_move_figure(from, to)):
		print("move performed")
	else:
		print("move not performed")
