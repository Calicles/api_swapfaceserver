# api_swapfaceserver
HTTP Appplication server to face swapping.
Handle OPTIONS, GET, POST.

Get:
- Have the count of the photo stores in the server.
-- usage []/swapFace/images_count
- Swap two image by their indexes.
-- usage []/swapFace/images/<<index in [1-images_count]>>
- Have a photo by its index.
-- usage []/swapFace/swap/index1/index2

Post:
Swap the photo sent in the request with the photo corresponding
with the index sent with.


