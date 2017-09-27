import i3s

  var refs : [Double] = [ 0.0, 0.0, 100.0, 100.0, 0.0, 100.0 ]
  var spots : [Double] = [
    10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0,
    20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
    30.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0,
    40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0
  ]
  var refs2 : [Double] = [ 0.0, 0.0, 100.0, 100.0, 0.0, 100.0 ]
  var spots2 : [Double] = [
    15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0,
    20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
    37.0, 37.0, 37.0, 37.0, 37.0, 37.0, 37.0, 37.0,
    40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0
  ]
  let count : Int32 = 4;

  var fgp = FingerPrint_new(&refs, &spots, count)
  var fgp2 = FingerPrint_new(&refs2, &spots2, count)
  
  Compare_two(fgp2, fgp)
  
  let score = FingerPrint_getScore(fgp)
  print("Score: " + String(score))

