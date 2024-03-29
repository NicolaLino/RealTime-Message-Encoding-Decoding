# Message Encoding/Decoding System :closed_lock_with_key:

This project aims to implement a multi-processing approach for sending encoded messages that can only be decoded by the intended receiver. The system involves the use of several processes, including a sender process, receiver process, master spy process, helper processes, and spy processes. The system operates under the assumption that a successful operation occurs when the receiver can decode the messages before the master spy, while a failed operation occurs if the master spy decodes the messages first.

## 🚀 Application Scenario :clipboard:

The system follows the following scenario:

1. A parent process creates the necessary child processes: a single sender process, a single receiver process, a single master spy process, a user-defined number of helper processes, and a user-defined number of spy processes.
2. The sender process retrieves the message from an input file (e.g., `sender.txt`). The message may contain multiple lines and paragraphs.
3. The sender process splits the input file into columns based on the blank character between words.
4. Each child process is responsible for encoding a specific column message received from the sender process. The encoding process is as follows:
   - For the first column, each word is modified by adding 1 to the first character modulo 26, 2 to the second character modulo 26, and so on. The encoded message is then placed in the first location of the shared memory.
   - For the second column, each word is modified by adding 2 to the first character modulo 26, 4 to the second character modulo 26, and so on. The encoded message is then placed in the second location of the shared memory.
   - The same logic applies to subsequent columns and words in each column.
   - Special characters and numbers are encoded based on specific rules mentioned in the project requirements.
   - Each encoded column in the shared memory is identified by a unique prefix or suffix.
5. Helper processes continuously swap the messages present in the shared memory to hinder spy processes from obtaining all the columns of the file.
6. Spy processes randomly access shared memory locations to retrieve encoded messages and send them to the master spy process.
7. The master spy process attempts to order the columns in the correct order after receiving them from the spy processes. It drops any columns it has already received. Once confident it has received all the columns, it tries to decode the messages present in the `spy.txt` file.
8. The receiver process also randomly accesses shared memory locations to retrieve encoded messages. Similar to the master spy process, it orders the received columns correctly and discards any duplicates. Once confident it has received all the columns, it attempts to decode the messages present in the `receiver.txt` file.
9. The parent process determines whether the receiver process was able to obtain the correct file from the sender before the master spy process. If the receiver succeeds, the operation is considered successful; otherwise, it is labeled as a failed operation.
10. The simulation ends if any of the following conditions are met:
    - The number of failed decoding operations by the receiver exceeds a user-defined threshold.
    - The number of successful decoding operations by the receiver exceeds a user-defined threshold.

## 🤖 Tech Stack

- Linux OS
- Docker
- VNC client
- C programming language
- OpenGL library

🚧 This project is a work in progress. Further updates and enhancements will be made.

# TODO List

- [x] Parent Process:
  - [x] Create shared memory for message exchange between processes.
  - [x] Create semaphores for synchronization and mutual exclusion.
  - [x] Create sender, receiver, master spy, helper, and spy processes.
  - [x] Wait for the receiver process to complete and determine the operation's success.

- [x] Sender Process:
  - [x] Read the message from the input file (sender.txt).
  - [x] Split the message into columns based on the blank character.
  - [x] Replace empty strings with "alright".
  - [x] Calculate the number of columns.
  - [x] Create child processes for each column and pass the column message to them.
  - [x] Encode the column message based on the specified rules.
  - [x] Place the encoded message in the shared memory.

- [x] Child Processes (Column Encoding):
  - [x] Receive the column message from the sender process.
  - [x] Encode the message according to the given rules.
  - [x] Add the encoded message to the shared memory.

- [x] Helper Processes:
  - [x] Continuously swap the messages present in the shared memory to confuse spy processes.
  - [x] Generate random locations in the shared memory to perform swapping.
  - [x] Use semaphores to ensure mutual exclusion during swapping.

- [x] Spy Processes:
  - [x] Continuously access random locations in the shared memory.
  - [x] Retrieve the encoded messages and send them to the master spy process.
  - [x] Use semaphores to coordinate access to shared memory.

- [x] Master Spy Process:
  - [x] Receive encoded messages from spy processes.
  - [x] Order the columns in the correct order.
  - [x] Drop columns that are already received.
  - [x] Determine when all columns are received.
  - [x] Attempt to decode the messages in the spy.txt file.
  - [x] Inform the parent process of the decoding result.

- [x] Receiver Process:
  - [x] Continuously access random locations in the shared memory.
  - [x] Retrieve the encoded messages.
  - [x] Order the columns in the correct order.
  - [x] Drop columns that are already received.
  - [x] Determine when all columns are received.
  - [x] Attempt to decode the messages in the receiver.txt file.
  - [x] Inform the parent process of the decoding result.

- [x] Parent Process (Continued):
  - [x] Wait for the receiver process to complete and receive the decoding result.
  - [x] Determine if the receiver successfully decoded the messages before the master spy.
  - [x] Label the operation as successful or failed based on the result.

- [x] Termination Condition:
  - [x] Monitor the number of failed decoding operations by the receiver process.
  - [x] Monitor the number of successful decoding operations by the receiver process.
  - [x] Terminate the simulation if either threshold is exceeded.
