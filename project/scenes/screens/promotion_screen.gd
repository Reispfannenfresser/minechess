extends Screen
class_name PromotionScreen

signal selected(figure_type: MineChessFigure.FigureType)

static var self_scene: PackedScene = preload("res://scenes/screens/promotion_screen.tscn")

static func create()-> PromotionScreen:
	var new_promotion_screen: PromotionScreen = self_scene.instantiate()
	return new_promotion_screen

func _on_queen_button_pressed()-> void:
	selected.emit(MineChessFigure.QUEEN)
	close()

func _on_rook_button_pressed()-> void:
	selected.emit(MineChessFigure.ROOK)
	close()

func _on_bishop_button_pressed()-> void:
	selected.emit(MineChessFigure.BISHOP)
	close()
	
func _on_knight_button_pressed()-> void:
	selected.emit(MineChessFigure.KNIGHT)
	close()
