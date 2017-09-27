import i3s_swift

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
  let count: Int = 4;

  var fgp = FingerPrint(ref: refs, data: spots, nr: count)
  var fgp2 = FingerPrint(ref: refs2, data: spots2, nr: count)
  
  fgp.compare(fgp2)
  
  let score = fgp.comparisonScore
  print("Score: " + String(score))
