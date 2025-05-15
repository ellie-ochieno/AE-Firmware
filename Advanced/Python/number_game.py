import random

def guess_number():
    print("Welcome to the Guess the Number Game!")
    print("Instructions: Guess a number between 1 and 100.")

    secret_number = random.randint(1, 100)
    attempts = 0

    while True:
        try:
            guess = int(input("\nEnter your guess: "))
            attempts += 1

            if guess < secret_number:
                print("Too low! Try a higher number.")
            elif guess > secret_number:
                print("Too high! Try a lower number.")
            else:
                print(f"\nCongratulations! You guessed the number {secret_number} correctly!")
                print(f"You took {attempts} attempts to guess the number.")
                break

        except ValueError:
            print("Invalid input. Please enter a valid number.")

    print("\nThank you for playing the Guess the Number Game. Goodbye!")

# Run the guess the number game
guess_number()
