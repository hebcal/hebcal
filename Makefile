BINARY_NAME=hebcal

$(BINARY_NAME): main.go gnu.go user.go
	go build

all: $(BINARY_NAME)

clean:
	go clean
	rm -f $(BINARY_NAME)
