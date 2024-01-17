package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"strconv"
	"strings"
	"time"

	"github.com/hebcal/greg"
	"github.com/hebcal/hdate"
	"github.com/MaxBGreenberg/hebcal-go/hebcal"
)

func readUserFile(filename string) []hebcal.UserEvent {
	f, err := os.Open(filename)
	if err != nil {
		fmt.Fprintf(os.Stderr, "could not open input file %s.\n", filename)
		os.Exit(1)
	}
	scanner := bufio.NewScanner(f)
	re := regexp.MustCompile(`^(\S+)\s+(\d+)\s+(.+)$`)
	lineNumber := 0
	entries := make([]hebcal.UserEvent, 0, 10)
	for scanner.Scan() {
		line0 := scanner.Text()
		line := strings.TrimSpace(line0)
		lineNumber++
		fields := re.FindStringSubmatch(line)
		if len(fields) != 4 {
			fmt.Fprintf(os.Stderr, "error, invalid format: %s:%d\n", filename, lineNumber)
			continue
		}
		month, err := hdate.MonthFromName(fields[1])
		if err != nil {
			fmt.Fprintf(os.Stderr, "error, invalid month: %s:%d\n", filename, lineNumber)
			continue
		}
		day, _ := strconv.Atoi(fields[2])
		if day < 1 || day > 30 {
			fmt.Fprintf(os.Stderr, "error, invalid days: %s:%d\n", filename, lineNumber)
		}
		entries = append(entries, hebcal.UserEvent{Month: month, Day: day, Desc: fields[3]})
	}
	return entries
}

func readYahrzeitFile(filename string) []hebcal.UserYahrzeit {
	f, err := os.Open(filename)
	if err != nil {
		fmt.Fprintf(os.Stderr, "could not open yahrtzeit input file %s.\n", filename)
		os.Exit(1)
	}
	scanner := bufio.NewScanner(f)
	re := regexp.MustCompile(`^(\d+)\s+(\d+)\s+(\d+)\s+(.+)$`)
	lineNumber := 0
	entries := make([]hebcal.UserYahrzeit, 0, 10)
	for scanner.Scan() {
		line0 := scanner.Text()
		line := strings.TrimSpace(line0)
		lineNumber++
		fields := re.FindStringSubmatch(line)
		if len(fields) != 5 {
			fmt.Fprintf(os.Stderr, "error, invalid format: %s:%d\n", filename, lineNumber)
			continue
		}
		month0, _ := strconv.Atoi(fields[1])
		if month0 < 1 || month0 > 12 {
			fmt.Fprintf(os.Stderr, "error, invalid month: %s:%d\n", filename, lineNumber)
			continue
		}
		day, _ := strconv.Atoi(fields[2])
		year, _ := strconv.Atoi(fields[3])
		month := time.Month(month0)
		if day < 1 || day > greg.DaysIn(month, year) {
			fmt.Fprintf(os.Stderr, "error, invalid days: %s:%d\n", filename, lineNumber)
		}
		gregDate := time.Date(year, month, day, 0, 0, 0, 0, time.UTC)
		entries = append(entries, hebcal.UserYahrzeit{Date: gregDate, Name: fields[4]})
	}
	return entries
}
