(let x 0)
(let cvs (gfx:new_canvas 512 512))

(= tico:update (fn (dt)
	; (let dx 0)
	(if (input:key_down "left") (= x (+ x 10)))
	(if (input:key_down "right") (= x (+ x 10)))
))


(= tico:draw (fn ()
	(cvs:attach)
	(gfx:fill_rect x 0 32 32)
	(cvs:detach)

	(cvs:draw 0 0)
))