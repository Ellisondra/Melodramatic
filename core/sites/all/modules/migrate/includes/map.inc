<?php

/**
 * @file
 * Defines the framework for map and message handling.
 */

/**
 * We implement the Iterator interface to support iteration over the map table
 * for the purpose of rollback.
 */
abstract class MigrateMap implements Iterator {
  /**
   * Arrays of key fields for the source and destination. Array keys are the
   * field names - array values are specific to the concrete map class.
   *
   * @var array
   */
  protected $sourceKey, $destinationKey;
  abstract public function getSourceKey();
  abstract public function getDestinationKey();

  /**
   * Mapping from field names to the map/message table key names (e.g.,
   * from input_field to sourceid1, or from nid to destid1)
   *
   * @var array
   */
  protected $sourceKeyMap, $destinationKeyMap;

  /**
   * Boolean determining whether to track last_imported times in map tables
   *
   * @var boolean
   */
  protected $trackLastImported = FALSE;

  /**
   * Save a mapping from the key values in the source row to the destination
   * keys.
   *
   * @param $source_row
   * @param $dest_ids
   * @param $needs_update
   */
  abstract public function saveIDMapping(stdClass $source_row, array $dest_ids,
    $needs_update = FALSE);

  /**
   * Record a message related to a source record
   *
   * @param array $source_key
   *  Source ID of the record in error
   * @param string $message
   *  The message to record.
   * @param int $level
   *  Optional message severity (defaults to MESSAGE_ERROR).
   */
  abstract public function saveMessage($source_key, $message, $level = MigrationBase::MESSAGE_ERROR);

  /**
   * Prepare to run a full update - mark all previously-imported content as
   * ready to be re-imported.
   */
  abstract public function prepareUpdate();

  /**
   * Report the number of imported items in the map
   */
  abstract public function importedCount();

  /**
   * Report the number of items that failed to import
   */
  abstract public function errorCount();

  /**
   * Report the number of messages
   */
  abstract public function messageCount();

  /**
   * Delete the map and message entries for a given source record
   *
   * @param array $source_key
   */
  abstract public function delete(array $source_key, $messages_only = FALSE);

  /**
   * Delete the map and message entries for a given destination record
   *
   * @param array $destination_key
   */
  abstract public function deleteDestination(array $destination_key);

  /**
   * Delete the map and message entries for a set of given source records.
   *
   * @param array $source_keys
   */
  abstract public function deleteBulk(array $source_keys);

  /**
   * Clear all messages from the map.
   */
  abstract public function clearMessages();

  /**
   * Retrieve map data for a given source or destination item
   */
  abstract public function getRowBySource(array $source_id);
  abstract public function getRowByDestination(array $destination_id);

  /**
   * Given a (possibly multi-field) destination key, return the (possibly multi-field)
   * source key mapped to it.
   *
   * @param array $destination_id
   *  Array of destination key values.
   * @return array
   *  Array of source key values, or NULL on failure.
   */
  abstract public function lookupSourceID(array $destination_id);

  /**
   * Given a (possibly multi-field) source key, return the (possibly multi-field)
   * destination key it is mapped to.
   *
   * @param array $source_id
   *  Array of source key values.
   * @return array
   *  Array of destination key values, or NULL on failure.
   */
  abstract public function lookupDestinationID(array $source_id);

  /**
   * Remove any persistent storage used by this map (e.g., map and message tables)
   */
  abstract public function destroy();
}
