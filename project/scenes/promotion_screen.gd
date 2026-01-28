extends Panel
class_name PromotionScreen

signal selected(figure_type: MineChessFigure.FigureType)

static var self_scene: PackedScene = preload("res://scenes/promotion_screen.tscn")

static func create()-> PromotionScreen:
	var new_promotion_screen: PromotionScreen = self_scene.instantiate()
	return new_promotion_screen

func _on_queen_button_pressed():
	emit_signal("selected", MineChessFigure.QUEEN)
	queue_free()

func _on_rook_button_pressed():
	emit_signal("selected", MineChessFigure.ROOK)
	queue_free()

func _on_bishop_button_pressed():
	emit_signal("selected", MineChessFigure.BISHOP)
	queue_free()
	
func _on_knight_button_pressed():
	emit_signal("selected", MineChessFigure.KNIGHT)
	queue_free()
