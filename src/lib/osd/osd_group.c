/*
 * 	miaofng@2010 initial version
 *		- stores all osd_xx_t object in flash
 *		- setup a fast link in memory for all dialog, all group and runtime items inside that group
 *		- do not construct several dialog at the same time unless they takes up different osd region
 */

#include "config.h"
#include "osd/osd_group.h"
#include "osd/osd_dialog.h"
#include "osd/osd_eng.h"
#include "FreeRTOS.h"
#include <stdlib.h>

/*
*	success return the new focused group handle
*	else return 0
*	note: focus order is not used yet
*/
int osd_SelectNextGroup(void) //change focus
{
	osd_dialog_k *kdlg = (osd_dialog_k *) osd_GetActiveDialog();
	osd_group_k *kgrp;
	
	if(kdlg == NULL)
		return 0;
	
	//find next and set focus tag
	kgrp = (kdlg->active_kgrp == NULL) ? kdlg->kgrps : kdlg->active_kgrp->next;
	for(; kgrp != NULL; kgrp = kgrp->next) {
		if(!osd_grp_select(kdlg, kgrp)) {
			break;
		}
	}

	return (kgrp == NULL);
}

int osd_SelectPrevGroup(void) //change focus
{
	osd_dialog_k *kdlg = (osd_dialog_k *) osd_GetActiveDialog();
	osd_group_k *kgrp;
	
	if(kdlg == NULL)
		return 0;
	
	//find next and set focus tag
	kgrp = (kdlg->active_kgrp == NULL) ? kdlg->kgrps : kdlg->active_kgrp->prev;
	for(; kgrp != NULL; kgrp = kgrp->prev) {
		if(!osd_grp_select(kdlg, kgrp)) {
			break;
		}
	}

	return (kgrp == NULL);
}

int osd_SelectGroup(const osd_group_t *grp)
{
	osd_dialog_k *kdlg = (osd_dialog_k *) osd_GetActiveDialog();
	osd_group_k *kgrp;
	
	if(kdlg == NULL)
		return 0;
	
	//find next and set focus tag
	kgrp = kdlg->kgrps;
	for(; kgrp != NULL; kgrp = kgrp->next) {
		if(kgrp -> grp == grp) {
			return osd_grp_select(kdlg, kgrp);
		}
	}

	return -1;
}

int osd_grp_select(struct osd_dialog_ks *kdlg, osd_group_k *kgrp)
{	
	if(kgrp -> status < STATUS_NORMAL)
		return -1;

	if(kdlg->active_kgrp != NULL) {
		kdlg->active_kgrp->focus = 0;
	}
	
	kgrp->focus = 1;
	kdlg->active_kgrp = kgrp;
	return 0;
}

osd_group_t *osd_GetCurrentGroup(void)
{
	const osd_group_t *grp = NULL;
	osd_dialog_k *kdlg = (osd_dialog_k *) osd_GetActiveDialog();
	if(kdlg) {
		grp = kdlg->active_kgrp->grp;
	}
	
	return (osd_group_t *)grp;
}

//group construct/destroy/show/hide ops
int osd_ConstructGroup(const osd_group_t *grp)
{
	osd_group_k *kgrp;

	kgrp = MALLOC(sizeof(osd_group_k));
	kgrp->grp = grp;
	kgrp->next = NULL;
	kgrp->prev = NULL;
	kgrp->status = osd_grp_get_status(grp);
	kgrp->focus = 0;

	osd_grp_get_rect(kgrp, &kgrp->margin);
	return (int)kgrp;
}

int osd_DestroyGroup(osd_group_k *kgrp)
{
	FREE(kgrp);
	return 0;
}

static int set_color(int status)
{
	int fg, bg;
	
	fg = (int) COLOR_FG_DEF;
	bg = (int) COLOR_BG_DEF;
	
	if(status == STATUS_FOCUSED) {
		fg = (int) COLOR_FG_FOCUS;
		bg = (int) COLOR_BG_FOCUS;
	}
	
	return osd_eng_set_color(fg, bg);
}

int osd_ShowGroup(osd_group_k *kgrp, int update)
{
	int status;
	const osd_group_t *grp;
	const osd_item_t *item;
	
	if(!osd_eng_is_visible(&kgrp -> margin))
		return 0;
	
	//evaluate new group status
	grp = kgrp->grp;
	status = osd_grp_get_status(grp);
	if(kgrp->focus)
		status = STATUS_FOCUSED;
	
	set_color(status);
	for(item = grp->items; (item != NULL) && (item->draw != NULL); item ++) {
		if(kgrp->status != status)
			osd_HideItem(item);
		osd_ShowItem(item, status);
	}
	
	kgrp->status = status;
	return 0;
}

int osd_HideGroup(osd_group_k *kgrp)
{
	int status;
	const osd_group_t *grp;
	const osd_item_t *item;
	
	if(!osd_eng_is_visible(&kgrp -> margin))
		return 0;

	//evaluate new group status
	grp = kgrp->grp;
	status = osd_grp_get_status(grp);
	if(kgrp->focus)
		status = STATUS_FOCUSED;

	set_color(status);
	kgrp->status = status;
	for(item = kgrp->grp->items; (item != NULL) && (item->draw != NULL); item ++)
		osd_HideItem(item);
			
	return 0;
}

int osd_grp_get_status(const osd_group_t *grp)
{
	int status = grp -> order;
	int (*get_status)(const osd_group_t *grp);
	
	if(grp->option & GROUP_RUNTIME_STATUS == GROUP_RUNTIME_STATUS) {
		get_status = (int (*)(const osd_group_t *)) grp -> order;
		status = get_status(grp);
	}
	
	if(status > STATUS_NORMAL)
		status = STATUS_NORMAL;
	return status;
}

rect_t *osd_grp_get_rect(const osd_group_k *kgrp, rect_t *margin)
{
	rect_t r;
	const osd_item_t *item;
	
	rect_zero(margin);
	for(item = kgrp->grp->items; (item != NULL) && (item->draw != NULL); item ++) {
		rect_set(&r, item->x, item->y, item->w, item->h);
		rect_merge(margin, &r);
	}
	return margin;
}

#ifdef CONFIG_DRIVER_PD
int osd_grp_react(osd_group_k *kgrp, int event, const dot_t *p)
{
	const osd_item_t *item;
	rect_t r;
	int event;
	
	//find the item to be responsible for the event
	for(item = kgrp->grp->items; (item != NULL) && (item->draw != NULL); item ++) {
		rect_set(&r, item->x, item->y, item->w, item->h);
#ifdef CONFIG_FONT_TNR08X16
		rect_zoom(&r, 3, 4);
#else
		rect_zoom(&r, 4, 5);
#endif
		if(rect_have(&r, p))
			break;
	}

	//item event?
	if(item != NULL)
		event = osd_item_react(item, event, p);
	
	//default to KEY_ENTER event
	return (event == OSDE_NONE) ? KEY_ENTER : event;
}
#endif
