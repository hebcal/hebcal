BINARY_NAME=hebcal

$(BINARY_NAME): main.go gnu.go user.go version.go
	go build -o $(BINARY_NAME)

all: $(BINARY_NAME)

clean:
	go clean
	rm -f $(BINARY_NAME)
