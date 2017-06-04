import sharkcwrapper

// Create shark; get pointer
var shark = Shark_new()

// Set shark ID number
Shark_set_id(shark, 71)

// Retreive and print shark ID number
var id = Shark_get_id(shark)
print("Shark id is \(id)")

// Destroy shark object
Shark_delete(shark)
