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

## 📝 To-Do List :pencil:

- [ ] Implement the parent process to create necessary child processes.
- [ ] Develop the sender process to retrieve and split the message into columns.
- [ ] Implement child processes to encode column messages and place them in the shared memory.
- [ ] Create helper processes to swap messages in the shared memory.
- [ ] Develop spy processes to access and send encoded messages to the master spy process.
- [ ] Implement the master spy process to order and decode received messages.
- [ ] Create the receiver process to access and decode messages from the shared memory.
- [ ] Develop the parent process to evaluate the success or failure of the operation.
- [ ] Handle termination conditions to end the simulation.
- [ ] Write unit tests to ensure the correctness of the implemented functionalities.
- [ ] Update the README file with detailed instructions on how to run the system.
- [ ] Write a comprehensive documentation describing the system's architecture, components, and functionality.

## 🤖 Tech Stack

- Linux OS
- Docker
- VNC client
- C programming language
- OpenGL library

🚧 This project is a work in progress. Further updates and enhancements will be made.

1. Parent Process Initialization:

    [x] Create a single sender process.
    [x] Create a single receiver process.
    [ ] Create a single master spy process.
    [ ] Create a user-defined number of helper processes.
    [ ] Create a user-defined number of spy processes.

2. Sender Process:

    [x] Read the message from an input file (e.g., sender.txt).
    [x] Split the message into columns based on the blank character between words.
    [x] Replace any empty strings in a column with the string "alright."
    [x] Create child processes for each column.
    [x] Send the column messages to the respective child processes.

3. Encoding Child Processes:

    [x] Receive the column message from the sender process.
    [x] Encode the column message based on the specified rules.
    [x] Place the encoded message in the shared memory at the appropriate location.

4. Helper Processes:

    [ ] Continuously swap messages in the shared memory to obfuscate the order.
    [ ] Randomly choose and swap the encoded messages between locations.

5. Spy Processes:

    [ ] Continuously access the shared memory locations randomly.
    [ ] Retrieve the encoded messages.
    [ ] Send the messages to the master spy process.

6. Master Spy Process:

    [ ] Receive encoded messages from the spy processes.
    [ ] Order the columns correctly, dropping duplicates.
    [ ] Attempt to decode the messages in the spy.txt file.
    [ ] Inform the parent process about the outcome.

7. Receiver Process:

    [ ] Continuously access the shared memory locations randomly.
    [ ] Retrieve the encoded messages.
    [ ] Order the columns correctly, dropping duplicates.
    [ ] Attempt to decode the messages in the receiver.txt file.
    [ ] Inform the parent process about the outcome.

8. Parent Process:

    [ ] Evaluate the decoding results of the receiver and master spy processes.
    [ ] Determine if the receiver successfully decoded the messages before the master spy.
    [ ] Label the operation as successful or failed based on the results.
    [ ] End the simulation if the user-defined thresholds for successful or failed operations are reached.
