extends Panel
class_name Screen

signal closed(screen: Screen)
signal cancelled(screen: Screen)

func close()-> void:
	closed.emit(self)
	queue_free()

func cancel()-> void:
	cancelled.emit(self)
	close()
