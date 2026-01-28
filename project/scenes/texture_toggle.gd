extends TextureRect

@export var button: BaseButton

@export var on_texture: Texture2D
@export var off_texture: Texture2D

func _ready():
	if (button.button_pressed):
		texture = on_texture
	else:
		texture = off_texture
	pass
	button.connect("toggled", _on_toggled)
	
func _on_toggled(toggled_on: bool):
	if (toggled_on):
		texture = on_texture
	else:
		texture = off_texture
