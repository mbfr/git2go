#include "_cgo_export.h"
#include <git2.h>
#include <git2/sys/odb_backend.h>
#include <git2/sys/refdb_backend.h>
#include <git2/sys/cred.h>

typedef int (*gogit_submodule_cbk)(git_submodule *sm, const char *name, void *payload);

void _go_git_apply_init_options(git_apply_options *options)
{
  git_apply_options opts = GIT_APPLY_OPTIONS_INIT;
  *options = opts;
}

void _go_git_populate_apply_cb(git_apply_options *options)
{
  options->delta_cb = (git_apply_delta_cb)deltaApplyCallback;
  options->hunk_cb = (git_apply_hunk_cb)hunkApplyCallback;
}

void _go_git_populate_commit_sign_cb(git_rebase_options *opts)
{
  opts->signing_cb = (git_commit_signing_cb)commitSignCallback;
}

void _go_git_populate_remote_cb(git_clone_options *opts)
{
	opts->remote_cb = (git_remote_create_cb)remoteCreateCallback;
}

void _go_git_populate_checkout_cb(git_checkout_options *opts)
{
	opts->notify_cb = (git_checkout_notify_cb)checkoutNotifyCallback;
	opts->progress_cb = (git_checkout_progress_cb)checkoutProgressCallback;
}

int _go_git_visit_submodule(git_repository *repo, void *fct)
{
	  return git_submodule_foreach(repo, (gogit_submodule_cbk)&SubmoduleVisitor, fct);
}

int _go_git_treewalk(git_tree *tree, git_treewalk_mode mode, void *ptr)
{
	return git_tree_walk(tree, mode, (git_treewalk_cb)&CallbackGitTreeWalk, ptr);
}

int _go_git_packbuilder_foreach(git_packbuilder *pb, void *payload)
{
    return git_packbuilder_foreach(pb, (git_packbuilder_foreach_cb)&packbuilderForEachCb, payload);
}

int _go_git_odb_foreach(git_odb *db, void *payload)
{
    return git_odb_foreach(db, (git_odb_foreach_cb)&odbForEachCb, payload);
}

void _go_git_odb_backend_free(git_odb_backend *backend)
{
    if (backend->free)
      backend->free(backend);

    return;
}

void _go_git_refdb_backend_free(git_refdb_backend *backend)
{
    if (backend->free)
      backend->free(backend);

    return;
}

int _go_git_diff_foreach(git_diff *diff, int eachFile, int eachHunk, int eachLine, void *payload)
{
	git_diff_file_cb fcb = NULL;
	git_diff_hunk_cb hcb = NULL;
	git_diff_line_cb lcb = NULL;

	if (eachFile) {
		fcb = (git_diff_file_cb)&diffForEachFileCb;
	}

	if (eachHunk) {
		hcb = (git_diff_hunk_cb)&diffForEachHunkCb;
	}

	if (eachLine) {
		lcb = (git_diff_line_cb)&diffForEachLineCb;
	}

	return git_diff_foreach(diff, fcb, NULL, hcb, lcb, payload);
}

int _go_git_diff_blobs(git_blob *old, const char *old_path, git_blob *new, const char *new_path, git_diff_options *opts, int eachFile, int eachHunk, int eachLine, void *payload)
{
	git_diff_file_cb fcb = NULL;
	git_diff_hunk_cb hcb = NULL;
	git_diff_line_cb lcb = NULL;

	if (eachFile) {
		fcb = (git_diff_file_cb)&diffForEachFileCb;
	}

	if (eachHunk) {
		hcb = (git_diff_hunk_cb)&diffForEachHunkCb;
	}

	if (eachLine) {
		lcb = (git_diff_line_cb)&diffForEachLineCb;
	}

	return git_diff_blobs(old, old_path, new, new_path, opts, fcb, NULL, hcb, lcb, payload);
}

void _go_git_setup_diff_notify_callbacks(git_diff_options *opts) {
	opts->notify_cb = (git_diff_notify_cb)diffNotifyCb;
}

void _go_git_setup_callbacks(git_remote_callbacks *callbacks) {
	typedef int (*completion_cb)(git_remote_completion_type type, void *data);
	typedef int (*update_tips_cb)(const char *refname, const git_oid *a, const git_oid *b, void *data);
	typedef int (*push_update_reference_cb)(const char *refname, const char *status, void *data);

	callbacks->sideband_progress = (git_transport_message_cb)sidebandProgressCallback;
	callbacks->completion = (completion_cb)completionCallback;
	callbacks->credentials = (git_cred_acquire_cb)credentialsCallback;
	callbacks->transfer_progress = (git_transfer_progress_cb)transferProgressCallback;
	callbacks->update_tips = (update_tips_cb)updateTipsCallback;
	callbacks->certificate_check = (git_transport_certificate_check_cb) certificateCheckCallback;
	callbacks->pack_progress = (git_packbuilder_progress) packProgressCallback;
	callbacks->push_transfer_progress = (git_push_transfer_progress) pushTransferProgressCallback;
	callbacks->push_update_reference = (push_update_reference_cb) pushUpdateReferenceCallback;
}

int _go_git_index_add_all(git_index *index, const git_strarray *pathspec, unsigned int flags, void *callback) {
	git_index_matched_path_cb cb = callback ? (git_index_matched_path_cb) &indexMatchedPathCallback : NULL;
	return git_index_add_all(index, pathspec, flags, cb, callback);
}

int _go_git_index_update_all(git_index *index, const git_strarray *pathspec, void *callback) {
	git_index_matched_path_cb cb = callback ? (git_index_matched_path_cb) &indexMatchedPathCallback : NULL;
	return git_index_update_all(index, pathspec, cb, callback);
}

int _go_git_index_remove_all(git_index *index, const git_strarray *pathspec, void *callback) {
	git_index_matched_path_cb cb = callback ? (git_index_matched_path_cb) &indexMatchedPathCallback : NULL;
	return git_index_remove_all(index, pathspec, cb, callback);
}

int _go_git_tag_foreach(git_repository *repo, void *payload)
{
    return git_tag_foreach(repo, (git_tag_foreach_cb)&gitTagForeachCb, payload);
}

int _go_git_merge_file(git_merge_file_result* out, char* ancestorContents, size_t ancestorLen, char* ancestorPath, unsigned int ancestorMode, char* oursContents, size_t oursLen, char* oursPath, unsigned int oursMode, char* theirsContents, size_t theirsLen, char* theirsPath, unsigned int theirsMode, git_merge_file_options* copts) {
	git_merge_file_input ancestor = GIT_MERGE_FILE_INPUT_INIT;
	git_merge_file_input ours = GIT_MERGE_FILE_INPUT_INIT;
	git_merge_file_input theirs = GIT_MERGE_FILE_INPUT_INIT;

	ancestor.ptr = ancestorContents;
	ancestor.size = ancestorLen;
	ancestor.path = ancestorPath;
	ancestor.mode = ancestorMode;

	ours.ptr = oursContents;
	ours.size = oursLen;
	ours.path = oursPath;
	ours.mode = oursMode;

	theirs.ptr = theirsContents;
	theirs.size = theirsLen;
	theirs.path = theirsPath;
	theirs.mode = theirsMode;

	return git_merge_file(out, &ancestor, &ours, &theirs, copts);
}

void _go_git_setup_stash_apply_progress_callbacks(git_stash_apply_options *opts) {
	opts->progress_cb = (git_stash_apply_progress_cb)stashApplyProgressCb;
}

int _go_git_stash_foreach(git_repository *repo, void *payload) {
    return git_stash_foreach(repo, (git_stash_cb)&stashForeachCb, payload);
}

int _go_git_writestream_write(git_writestream *stream, const char *buffer, size_t len)
{
	return stream->write(stream, buffer, len);
}

int _go_git_writestream_close(git_writestream *stream)
{
	return stream->close(stream);
}

void _go_git_writestream_free(git_writestream *stream)
{
	stream->free(stream);
}

git_credtype_t _go_git_cred_credtype(git_cred *cred) {
	return cred->credtype;
}

int _go_git_odb_write_pack(git_odb_writepack **out, git_odb *db, void *progress_payload)
{
	return git_odb_write_pack(out, db, (git_transfer_progress_cb)transferProgressCallback, progress_payload);
}

int _go_git_odb_writepack_append(git_odb_writepack *writepack, const void *data, size_t size, git_transfer_progress *stats)
{
	return writepack->append(writepack, data, size, stats);
}

int _go_git_odb_writepack_commit(git_odb_writepack *writepack, git_transfer_progress *stats)
{
	return writepack->commit(writepack, stats);
}

void _go_git_odb_writepack_free(git_odb_writepack *writepack)
{
	writepack->free(writepack);
}

int _go_git_indexer_new(git_indexer **out, const char *path, unsigned int mode, git_odb *odb, void *progress_cb_payload)
{
	git_indexer_options indexer_options = GIT_INDEXER_OPTIONS_INIT;
	indexer_options.progress_cb = (git_transfer_progress_cb)transferProgressCallback;
	indexer_options.progress_cb_payload = progress_cb_payload;
	return git_indexer_new(out, path, mode, odb, &indexer_options);
}

/* EOF */
